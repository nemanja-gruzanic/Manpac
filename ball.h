/* plotting main object */
static double x_curr, z_curr;

static void ball(){
  glPushMatrix();
  glColor3f(1,1,0);
  glTranslated(x_curr,0.08,z_curr);
  glutSolidSphere(0.1,40,40);
  glPopMatrix();
}
