#include "glview.h"

GLView::GLView()
{
    zoom = -50;
    xScreen = 0;
    yScreen = 0;
    rotX = rotY = rotZ = 0;
    isDrawNet = true;
    isDrawXyz = false;
    drawType = 0;

    triangulation = new Triangulation;

    object = 0;
    clearColor = 0.1;

    tetgen = true;
    newObject = false;
}

GLView::~GLView()
{

}


void GLView::initializeGL()
{
    glClearColor(0.1, 0.1, 0.1, 0.0);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
void GLView::resizeGL(int width, int height)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport (0, 0, width, height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (45, (GLfloat)width/(GLfloat)height, 0.01f, 1000.0f);
    glMatrixMode (GL_MODELVIEW);
}

void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(xScreen, yScreen, zoom);
    glRotated(20,1,0,0);

    glRotated (rotX/16.0, 1.0, 0.0, 0.0);
    glRotated (rotY/16.0, 0.0, 1.0, 0.0);
    glRotated (rotZ/16.0, 0.0, 0.0, 1.0);

    glClearColor(clearColor,clearColor,clearColor,0.0);

    makeObject();
}


void GLView::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360*16;
    while (*angle > 360*16)
        *angle -= 360*16;
}
void GLView::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}
void GLView::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        setXRotation (rotX + 8 * dy);
        setYRotation (rotY + 8 * dx);
    }
    else
        if (event->buttons() & Qt::RightButton)
        {
            setXRotation (rotX + 8 * dy);
            setZRotation (rotZ + 8 * dx);
        }
        else
        if(event->buttons() & Qt::MidButton)
        {
                int tmp = zoom;
            zoom = zoom - dy;
                if(zoom > -1)
                {
                    zoom = tmp;
                    return;
                }
                if(zoom < -280)
                {
                    zoom = tmp;
                    return;
                }
                updateGL();
        }
    lastPos = event->pos();

}
void GLView::wheelEvent(QWheelEvent *event)
{
    int tmp = zoom;
    zoom += event->delta()/80;
    if(zoom > -1)
    {
        zoom = tmp;
        return;
    }
    if(zoom < -280)
    {
        zoom = tmp;
        return;
    }
    event->accept();
    updateGL();
//    emit zoomChanged();
}
void GLView::setXRotation (int angle)
{
    normalizeAngle(&angle);
        rotX = angle;
        updateGL();
}
void GLView::setYRotation (int angle)
{
    normalizeAngle(&angle);
        rotY = angle;
        updateGL();
}
void GLView::setZRotation (int angle)
{
    normalizeAngle(&angle);
        rotZ = angle;
        updateGL();
}


void GLView::drawNet()
{
    glPushMatrix();
    glRotated(-20, 0,1,0);

    glLineWidth(1);
    glColor3f (0.3f, 0.3f, 0.3f);
    for(int i=-50; i<=50; i=i+2)
    {
        glBegin (GL_LINES);
            glVertex3f (i, 0.0, -50.0);
            glVertex3f (i, 0.0, 50.0);
        glEnd ();
        glBegin (GL_LINES);
            glVertex3f (-50, 0.0, i);
            glVertex3f (50, 0.0, i);
        glEnd ();
    }

    glPopMatrix();
}
void GLView::drawXyz()
{
    glPushMatrix();
    glRotated(-20, 0,1,0);

    glLineWidth(1);
    glColor3f(0.8, 0, 0);
    glBegin (GL_LINES);
        glVertex3f (0.0, 0.0, 0.0);
        glVertex3f (25.0, 0.0, 0.0);
    glEnd ();
    glColor3f(0, 0.8, 0);
    glBegin (GL_LINES);
        glVertex3f (0.0, 0.0, 0.0);
        glVertex3f (0.0, 25.0, 0.0);
    glEnd ();
    glColor3f(0, 0, 0.8);
    glBegin (GL_LINES);
        glVertex3f (0.0, 0.0, 0.0);
        glVertex3f (0.0, 0.0, 25.0);
    glEnd ();

    glPopMatrix();
}

void GLView::makeObject()
{
    int k;
    int currentSelected = 0;

    if(isDrawNet)
        drawNet();
    if(isDrawXyz)
        drawXyz();

    if(!tetgen)
    {
        switch(drawType)
        {
        case 1:
            if(newObject)
            {
                if(object != 0)
                    glDeleteLists(object,1);
                object = glGenLists(1);

                glNewList(object, GL_COMPILE);
                    glPushMatrix();
                    glRotated(-20, 0,1,0);
                    for(int i=0; i<listId.count(); i++)
                    {
                        glColor3d((GLdouble)listColorsR.at(i),(GLdouble)listColorsG.at(i),(GLdouble)listColorsB.at(i));
                        glBegin(GL_POINTS);
                            glVertex3d((GLdouble)listPointsX.at(i),(GLdouble)listPointsY.at(i),(GLdouble)listPointsZ.at(i));
                        glEnd();

                        if(!selectedPoints.isEmpty() && currentSelected<selectedPoints.count())
                        {
                            if(selectedPoints.at(currentSelected) == i)
                            {
                                glPushMatrix();
                                glColor3f (0.9f, 0.0f, 0.0f);
                                    GLUquadricObj *qobj;
                                    qobj = gluNewQuadric();

                                    gluQuadricDrawStyle( qobj, GLU_FILL );
                                    gluQuadricNormals( qobj, GLU_FLAT );

                                    glTranslated((GLdouble)listPointsX.at(i),(GLdouble)listPointsY.at(i),(GLdouble)listPointsZ.at(i));
                                    gluSphere(qobj,0.1,8,8);

                                    gluDeleteQuadric(qobj);
                                glPopMatrix();
                                currentSelected++;
                            }
                        }
                    }
                    glPopMatrix();
                glEndList();
            }

            glCallList(object);
            break;
        case 2:
            if(newObject)
            {
                line.clear();
                line = triangulation->getLine(listId,listPointsX,listPointsY,listPointsZ,listColorsR,listColorsG,listColorsB);

                if(object != 0)
                    glDeleteLists(object,1);
                object = glGenLists(1);

                glNewList(object, GL_COMPILE);
                    glPushMatrix();
                    glRotated(-20, 0,1,0);
                    k=0;
                    while(k<line.count())
                    {
                        glBegin(GL_LINES);
                            glColor3d((GLdouble)line[k],(GLdouble)line[k+1],(GLdouble)line[k+2]);
                            k+=3;
                            glVertex3d((GLdouble)line[k],(GLdouble)line[k+1],(GLdouble)line[k+2]);
                            k+=3;
                            glColor3d((GLdouble)line[k],(GLdouble)line[k+1],(GLdouble)line[k+2]);
                            k+=3;
                            glVertex3d((GLdouble)line[k],(GLdouble)line[k+1],(GLdouble)line[k+2]);
                            k+=3;
                        glEnd();
                    }
                    glPopMatrix();
                glEndList();
            }
            glCallList(object);
            break;
        case 3:
            if(newObject)
            {
                triangle.clear();
                triangle = triangulation->getTriangle(listId,listPointsX,listPointsY,listPointsZ,listColorsR,listColorsG,listColorsB);

                if(object != 0)
                    glDeleteLists(object,1);
                object = glGenLists(1);

                glNewList(object, GL_COMPILE);
                    glPushMatrix();
                    glRotated(-20, 0,1,0);
                    k=0;
                    while(k<triangle.count())
                    {
                        glBegin(GL_TRIANGLE_STRIP);
                            glColor3d((GLdouble)triangle[k],(GLdouble)triangle[k+1],(GLdouble)triangle[k+2]);
                            k+=3;
                            glVertex3d((GLdouble)triangle[k],(GLdouble)triangle[k+1],(GLdouble)triangle[k+2]);
                            k+=3;
                            glColor3d((GLdouble)triangle[k],(GLdouble)triangle[k+1],(GLdouble)triangle[k+2]);
                            k+=3;
                            glVertex3d((GLdouble)triangle[k],(GLdouble)triangle[k+1],(GLdouble)triangle[k+2]);
                            k+=3;
                            glColor3d((GLdouble)triangle[k],(GLdouble)triangle[k+1],(GLdouble)triangle[k+2]);
                            k+=3;
                            glVertex3d((GLdouble)triangle[k],(GLdouble)triangle[k+1],(GLdouble)triangle[k+2]);
                            k+=3;
                        glEnd();
                    }
                    glPopMatrix();
                glEndList();
            }
            glCallList(object);
            break;
        default:
            break;
        }
    }
    else      //#############################    TETGEN: #############################################
    {
        if(newObject)
        {
            QString poly, color;
            poly = "# Part 1 - node list\n";
            poly.append(QString::number(listPointsX.count()) + " 3 0 0\n");
            for(int i=1; i<=listPointsX.count(); i++)
            {
                poly.append(QString::number(i) + "\t" + QString::number(listPointsX[i-1]) + " " + QString::number(listPointsY[i-1]) + " " + QString::number(listPointsZ[i-1]) + "\n");
                color.append(QString::number(i) + "\t" + QString::number(listColorsR[i-1]) + " " + QString::number(listColorsG[i-1]) + " " + QString::number(listColorsB[i-1]) + "\n");
            }
            poly.append("\n# Part 2 - facet list\n0\n\n# Part 3 - hole list\n0\n\n# Part 4 - region list\n0");


            QFile filePoly("tmp.poly");
            QFile fileColor("tmp.color");
            if(filePoly.open(QFile::WriteOnly | QFile::Text))
            {
                 QTextStream out(&filePoly);
                 out << poly;
            }
            filePoly.close();
            if(fileColor.open(QFile::WriteOnly | QFile::Text))
            {
                 QTextStream out(&fileColor);
                 out << color;
            }
            fileColor.close();

            QString command = "./startTetGen";
            system(command.toLatin1());


            QStringList listFace, listColor, listNode;
            QFile *fF = new QFile("tmp.1.face");
            if(!fF->open(QIODevice::ReadOnly | QFile::Text))
            {
                QMessageBox::warning(this, "Tworzenie obiektu", "Błąd IO(1)");
                return;
            }
            QTextStream inF(fF);
            while(!inF.atEnd())
            {
                listFace.append(inF.readLine());
            }
            fF->close();
            delete fF;

            QFile *fC = new QFile("tmp.color");
            if(!fC->open(QIODevice::ReadOnly | QFile::Text))
            {
                QMessageBox::warning(this, "Tworzenie obiektu", "Błąd IO(2)");
                return;
            }
            QTextStream inC(fC);
            while(!inC.atEnd())
            {
                listColor.append(inC.readLine());
            }
            fC->close();
            delete fC;

            QFile *fN = new QFile("tmp.poly");
            if(!fN->open(QIODevice::ReadOnly | QFile::Text))
            {
                QMessageBox::warning(this, "Tworzenie obiektu", "Błąd IO(3)");
                return;
            }
            QTextStream inN(fN);
            while(!inN.atEnd())
            {
                listNode.append(inN.readLine());
            }
            fN->close();
            delete fN;

            if(listFace.count() < 8 || listColor.count() < 2 || listNode.count() < 10)
                return;

            listFace.removeFirst();
            listFace.removeLast();
    //        listColor.removeLast();
            listNode.removeFirst();
            listNode.removeFirst();
            listNode.removeLast();
            listNode.removeLast();
            listNode.removeLast();
            listNode.removeLast();
            listNode.removeLast();
            listNode.removeLast();
            listNode.removeLast();
            listNode.removeLast();
            listNode.removeLast();

            int tmp1, tmp2, tmp3; // Punkty trójkąta
            double tmpX, tmpY, tmpZ; // Współrzędne
            double tmpR, tmpG, tmpB; // Kolor


            if(object != 0)
                glDeleteLists(object,1);
            object = glGenLists(1);
            glNewList(object, GL_COMPILE);

            glBegin(GL_TRIANGLE_STRIP);
                for(int i=0; i<listFace.count(); i++)
                {
                    tmp1 = (int)foundText(1, listFace.at(i));
                    tmp2 = (int)foundText(2, listFace.at(i));
                    tmp3 = (int)foundText(3, listFace.at(i));

                    tmpX = foundText(1, listNode.at(tmp1-1));
                    tmpY = foundText(2, listNode.at(tmp1-1));
                    tmpZ = foundText(3, listNode.at(tmp1-1));
                    tmpR = foundText(1, listColor.at(tmp1-1));
                    tmpG = foundText(2, listColor.at(tmp1-1));
                    tmpB = foundText(3, listColor.at(tmp1-1));
                        glColor3d(tmpR, tmpG, tmpB);
                        glVertex3d(tmpX, tmpY, tmpZ);
                    tmpX = foundText(1, listNode.at(tmp2-1));
                    tmpY = foundText(2, listNode.at(tmp2-1));
                    tmpZ = foundText(3, listNode.at(tmp2-1));
                    tmpR = foundText(1, listColor.at(tmp2-1));
                    tmpG = foundText(2, listColor.at(tmp2-1));
                    tmpB = foundText(3, listColor.at(tmp2-1));
                        glColor3d(tmpR, tmpG, tmpB);
                        glVertex3d(tmpX, tmpY, tmpZ);
                    tmpX = foundText(1, listNode.at(tmp3-1));
                    tmpY = foundText(2, listNode.at(tmp3-1));
                    tmpZ = foundText(3, listNode.at(tmp3-1));
                    tmpR = foundText(1, listColor.at(tmp3-1));
                    tmpG = foundText(2, listColor.at(tmp3-1));
                    tmpB = foundText(3, listColor.at(tmp3-1));
                        glColor3d(tmpR, tmpG, tmpB);
                        glVertex3d(tmpX, tmpY, tmpZ);
                }
            glEnd();

            glEndList();

            QDir d;
            d.remove("tmp.poly");
            d.remove("tmp.color");
            d.remove("tmp.1.ele");
            d.remove("tmp.1.face");
            d.remove("tmp.1.node");
        }
        glCallList(object);

    }

    newObject = false;
}


double GLView::foundText(int param, QString text)
{
    QStringList list;
    list = text.split(QRegularExpression("\\s+"));

    return list.at(param).toDouble();
}




void GLView::setTetgen(bool b)
{
    tetgen = b;

//    updateGL();
}

