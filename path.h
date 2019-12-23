static void path(){
    glColor3f(0, 0, 1);
    glBegin(GL_POLYGON);
    glVertex3f(-1, 0, 100);
    glVertex3f(1, 0, 100);
    glVertex3f(1, 0, -100);
    glVertex3f(-1, 0, -100);
    glEnd();
}
