/*****************************************************************************\
 * TP CPE, 4ETI, TP synthese d'images
 * --------------
 *
 * Programme principal des appels OpenGL
 \*****************************************************************************/

#include "declaration.h"
//#include <unistd.h>
 

//identifiant des shaders
GLuint shader_program_id;
GLuint gui_program_id;

camera cam;
int temps = 0;
int score = 0;


const int nb_obj = 13;
const int nb_mur = 3;
const int nb_matrice = 3;
int matrice[nb_matrice][nb_obj][nb_obj];

objet3d obj[nb_obj][nb_obj][nb_mur];

int config[nb_mur];

const int nb_obj2 = 6;
objet3d obj2[nb_obj2];

const int nb_text = 3;
text text_to_draw[nb_text];

float angle_z_model_1 = 0.0f;
float angle_z_model_5 = 0.0f;

unsigned int sleep (unsigned int nb_sec);

/*****************************************************************************\
* initialisation                                                              *
\*****************************************************************************/
static void init()
{
  shader_program_id = glhelper::create_program_from_file("shaders/shader.vert", "shaders/shader.frag"); CHECK_GL_ERROR();

  cam.projection = matrice_projection(60.0f*M_PI/180.0f,1.0f,0.01f,100.0f);

   
  //cam.tr.translation = vec3(0.0f, 1.0f, 0.0f);

  //cam.tr.rotation_center = vec3(3.0f, 6.0f, 0.0f);

   //cam.tr.rotation_euler = vec3(0.0f, M_PI/2., 0.0f);

  init_model_2();
  init_model_3();
  init_model_4();
  init_model_5();
  //init_model_6();
  cam.tr.translation = obj2[1].tr.translation +vec3(0.0f, 2.0f, 4.0f) ;
  cam.tr.rotation_euler = vec3(M_PI/12,0.0f,0.0f);

  gui_program_id = glhelper::create_program_from_file("shaders/gui.vert", "shaders/gui.frag"); CHECK_GL_ERROR();

text_to_draw[0].value = "Le Best Binome";
text_to_draw[0].bottomLeft = vec2(-0.2, 0.5);
text_to_draw[0].topRight = vec2(0.2, 1);

text_to_draw[2].value = "Score";
text_to_draw[2].bottomLeft = vec2(0.8, 0.8);
text_to_draw[2].topRight = vec2(1, 1.4);

init_text(text_to_draw);


}

/*****************************************************************************\
* display_callback                                                           *
\*****************************************************************************/
 static void display_callback()
{
 glClearColor(0.5f, 0.6f, 0.9f, 1.0f); CHECK_GL_ERROR();
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERROR();


 for (int i = 0; i < nb_obj2; ++i) {
     draw_obj3d(obj2 + i, cam);
 }

    for(int k = 0; k < nb_mur; ++k){
        for(int i = 0; i < nb_obj; ++i){
            for(int j = 0; j < nb_obj; ++j){

            draw_obj3d(&obj[i][j][k] , cam);
   
            }
        }
    }

    for(int i = 0; i < nb_text; ++i){
    draw_text(text_to_draw + i);
    }

  //mise à jour du timer 
  temps++;
  text_to_draw[1] = text_to_draw[0];
  //Ici le temps est incrémenté toutes les 25 ms, donc on le divise par 40 pour afficher les secondes.
  text_to_draw[1].value = std::to_string(temps/40);
  text_to_draw[1].bottomLeft = vec2(-1, 0.90);
  text_to_draw[1].topRight = vec2(-0.90, 1);

  //Gestion du score
  text_to_draw[2] = text_to_draw[0];
  text_to_draw[2].value = "Score :"+ std::to_string(score) + std::to_string(((obj2[1].tr.translation.x) / 0.4));
  text_to_draw[2].bottomLeft = vec2(0.5, 0.0);
  text_to_draw[2].topRight = vec2(1, 0.9);

for (int i=0; i<nb_mur; i++){
  for (int j =0; j<nb_obj;j++){
    for (int k =0; k<nb_obj; k++){
      


      if (obj[j][k][i].tr.translation.z > obj2[1].tr.translation.z+0.6  ){
        obj[j][k][i].tr.translation.z = -31;
      }
    }
  }
}

//Teste si le personnage est proche du mur (va permettre d'analyser sa position)
if (abs(obj[0][0][0].tr.translation.z - obj2[1].tr.translation.z ) < 0.012) {
    if (abs(((obj2[1].tr.translation.x) / 0.4) - 6) < 0.1){
        score++;
    }
}



  glutSwapBuffers();
}

/*****************************************************************************\
* keyboard_callback                                                           *
\*****************************************************************************/
static void keyboard_callback(unsigned char key, int, int)
{
  switch (key)
  {
    case 'p':
      glhelper::print_screen();
      break;
    case 'q':
    case 'Q':

    case 27:
      exit(0);
      break;
    case 'l':
      angle_z_model_1 -= M_PI/6;
    break;
    case 'o':
      angle_z_model_5 += M_PI/6;
    break;
  }
  obj2[2].tr.rotation_euler =vec3 (0.0f,0.0f,angle_z_model_1);
  obj2[3].tr.rotation_euler =vec3 (0.0f,0.0f,angle_z_model_5);
}

/***************************************************************************\
* special_callback                                                            *
\*****************************************************************************/
static void special_callback(int key, int, int)
{
  float dL=0.04f;
  switch (key)
  {
    case GLUT_KEY_UP:
      obj2[1].tr.translation.y += dL; //rotation avec la touche du haut
      obj2[2].tr.translation.y += dL;
      obj2[3].tr.translation.y += dL;

      break;
    case GLUT_KEY_DOWN:
      obj2[1].tr.translation.y -= dL; //rotation avec la touche du bas
      obj2[2].tr.translation.y -= dL;
      obj2[3].tr.translation.y -= dL;

      break;
    case GLUT_KEY_LEFT:
      obj2[1].tr.translation.x -= dL; //rotation avec la touche de gauche
      obj2[2].tr.translation.x -= dL;
      obj2[3].tr.translation.x -= dL;

      break;
    case GLUT_KEY_RIGHT:
      obj2[1].tr.translation.x += dL; //rotation avec la touche de droite
      obj2[2].tr.translation.x += dL;
      obj2[3].tr.translation.x += dL;

      break;
  }
    //déplacement de la caméra en même temps que le mouvement du personnage
  cam.tr.translation = obj2[1].tr.translation   +vec3(0.0f, 2.0f, 4.0f);
  cam.tr.rotation_euler = vec3(M_PI/12,0.0f,0.0f);
}


/*****************************************************************************\
* timer_callback                                                              *
\*****************************************************************************/
static void timer_callback(int)
{

for (int k =0; k<nb_mur; k++){
  for(int i = 0; i < nb_obj; ++i){
    for(int j = 0; j < nb_obj; ++j){
      obj[i][j][k].tr.translation.z+=0.3;
    }
  }
}
/*

if (temps == 600){
  for (int i=0; i<nb_mur; i++){  
    for (int k = 0; k<nb_obj; k++){
      for(int j = 0; j<nb_obj; j++){
        obj[k][j][i].visible = true;
      }}}
}

if (temps == 1000){
  for (int i=0; i<nb_mur; i++){  
    for (int k = 0; k<nb_obj; k++){
      for(int j = 0; j<nb_obj; j++){
        

        

        if (i==0){
          if (!matrice[i][k][j]) {
            obj[k][j][i].visible = false;
          }
        }
        if (i == 1) {
          if (!matrice[i][k][j]) {
            obj[k][j][i].visible = false;
          }
        }
        if (i == 2) {
          if (!matrice[i][k][j]) {
            obj[k][j][i].visible = false;
          }
        }

    obj[k][j][i].prog = shader_program_id;
    obj[k][j][i].tr.translation = vec3(0.0 +0.4*j, 0.0+0.4*k, -10.0-i*7);

    }
  }
}
}
*/
    

  glutTimerFunc(25, timer_callback, 0);
  glutPostRedisplay();
}

/*****************************************************************************\
* main                                                                         *
\***************************************  sleep(2);
\***************************************  sleep(2);


  init_model_3();**************************************/
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | MACOSX_COMPATIBILITY);
  glutInitWindowSize(600, 600);
  glutCreateWindow("OpenGL");

  glutDisplayFunc(display_callback);
  glutKeyboardFunc(keyboard_callback);
  glutSpecialFunc(special_callback);
  glutTimerFunc(25, timer_callback, 0);

  glewExperimental = true;
  glewInit();

  std::cout << "OpenGL: " << (GLchar *)(glGetString(GL_VERSION)) << std::endl;

  init();
  glutMainLoop();

  return 0;
}

/*****************************************************************************\
* draw_text                                                                   *
\*****************************************************************************/
void draw_text(const text * const t)
{
  if(!t->visible) return;
  
  glDisable(GL_DEPTH_TEST);
  glUseProgram(t->prog);

  vec2 size = (t->topRight - t->bottomLeft) / float(t->value.size());
  
  GLint loc_size = glGetUniformLocation(gui_program_id, "size"); CHECK_GL_ERROR();
  if (loc_size == -1) std::cerr << "Pas de variable uniforme : size" << std::endl;
  glUniform2f(loc_size,size.x, size.y);     CHECK_GL_ERROR();

  glBindVertexArray(t->vao);                CHECK_GL_ERROR();
  
  for(unsigned i = 0; i < t->value.size(); ++i)
  {
    GLint loc_start = glGetUniformLocation(gui_program_id, "start"); CHECK_GL_ERROR();
    if (loc_start == -1) std::cerr << "Pas de variable uniforme : start" << std::endl;
    glUniform2f(loc_start,t->bottomLeft.x+i*size.x, t->bottomLeft.y);    CHECK_GL_ERROR();

    GLint loc_char = glGetUniformLocation(gui_program_id, "c"); CHECK_GL_ERROR();
    if (loc_char == -1) std::cerr << "Pas de variable uniforme : c" << std::endl;
    glUniform1i(loc_char, (int)t->value[i]);    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, t->texture_id);                            CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);                    CHECK_GL_ERROR();
  }
}

/*****************************************************************************\
* draw_obj3d                                                                  *
\*****************************************************************************/
void draw_obj3d(const objet3d* const obj, camera cam)
{
  if(!obj->visible) return;

  glEnable(GL_DEPTH_TEST);
  glUseProgram(obj->prog);
  
  {
    GLint loc_projection = glGetUniformLocation(shader_program_id, "projection"); CHECK_GL_ERROR();
    if (loc_projection == -1) std::cerr << "Pas de variable uniforme : projection" << std::endl;
    glUniformMatrix4fv(loc_projection,1,false,pointeur(cam.projection));    CHECK_GL_ERROR();

    GLint loc_rotation_view = glGetUniformLocation(shader_program_id, "rotation_view"); CHECK_GL_ERROR();
    if (loc_rotation_view == -1) std::cerr << "Pas de variable uniforme : rotation_view" << std::endl;
    mat4 rotation_x = matrice_rotation(cam.tr.rotation_euler.x, 1.0f, 0.0f, 0.0f);
    mat4 rotation_y = matrice_rotation(cam.tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
    mat4 rotation_z = matrice_rotation(cam.tr.rotation_euler.z, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(loc_rotation_view,1,false,pointeur(rotation_x*rotation_y*rotation_z));    CHECK_GL_ERROR();

    vec3 cv = cam.tr.rotation_center;
    GLint loc_rotation_center_view = glGetUniformLocation(shader_program_id, "rotation_center_view"); CHECK_GL_ERROR();
    if (loc_rotation_center_view == -1) std::cerr << "Pas de variable uniforme : rotation_center_view" << std::endl;
    glUniform4f(loc_rotation_center_view , cv.x,cv.y,cv.z , 0.0f); CHECK_GL_ERROR();

    vec3 tv = cam.tr.translation;
    GLint loc_translation_view = glGetUniformLocation(shader_program_id, "translation_view"); CHECK_GL_ERROR();
    if (loc_translation_view == -1) std::cerr << "Pas de variable uniforme : translation_view" << std::endl;
    glUniform4f(loc_translation_view , tv.x,tv.y,tv.z , 0.0f); CHECK_GL_ERROR();
  }
  {
    GLint loc_rotation_model = glGetUniformLocation(obj->prog, "rotation_model"); CHECK_GL_ERROR();
    if (loc_rotation_model == -1) std::cerr << "Pas de variable uniforme : rotation_model" << std::endl;
    mat4 rotation_x = matrice_rotation(obj->tr.rotation_euler.x, 1.0f, 0.0f, 0.0f);
    mat4 rotation_y = matrice_rotation(obj->tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
    mat4 rotation_z = matrice_rotation(obj->tr.rotation_euler.z, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(loc_rotation_model,1,false,pointeur(rotation_x*rotation_y*rotation_z));    CHECK_GL_ERROR();

    vec3 c = obj->tr.rotation_center;
    GLint loc_rotation_center_model = glGetUniformLocation(obj->prog, "rotation_center_model");   CHECK_GL_ERROR();
    if (loc_rotation_center_model == -1) std::cerr << "Pas de variable uniforme : rotation_center_model" << std::endl;
    glUniform4f(loc_rotation_center_model , c.x,c.y,c.z , 0.0f);                                  CHECK_GL_ERROR();

    vec3 t = obj->tr.translation;
    GLint loc_translation_model = glGetUniformLocation(obj->prog, "translation_model"); CHECK_GL_ERROR();
    if (loc_translation_model == -1) std::cerr << "Pas de variable uniforme : translation_model" << std::endl;
    glUniform4f(loc_translation_model , t.x,t.y,t.z , 0.0f);                                     CHECK_GL_ERROR();
  }
  glBindVertexArray(obj->vao);                                              CHECK_GL_ERROR();

  glBindTexture(GL_TEXTURE_2D, obj->texture_id);                            CHECK_GL_ERROR();
  glDrawElements(GL_TRIANGLES, 3*obj->nb_triangle, GL_UNSIGNED_INT, 0);     CHECK_GL_ERROR();

}

void init_text(text *t){
  vec3 p0=vec3( 0.0f, 0.0f, 0.0f);
  vec3 p1=vec3( 0.0f, 1.0f, 0.0f);
  vec3 p2=vec3( 1.0f, 1.0f, 0.0f);
  vec3 p3=vec3( 1.0f, 0.0f, 0.0f);

  vec3 geometrie[4] = {p0, p1, p2, p3}; 
  triangle_index index[3] = { triangle_index(0, 1, 2), triangle_index(0, 2, 3), triangle_index(1, 2, 3)};

  glGenVertexArrays(1, &(t->vao));                                              CHECK_GL_ERROR();
  glBindVertexArray(t->vao);                                                  CHECK_GL_ERROR();

  GLuint vbo;
  glGenBuffers(1, &vbo);                                                       CHECK_GL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER,vbo);                                          CHECK_GL_ERROR();
  glBufferData(GL_ARRAY_BUFFER,sizeof(geometrie),geometrie,GL_STATIC_DRAW);   CHECK_GL_ERROR();

  glEnableVertexAttribArray(0); CHECK_GL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); CHECK_GL_ERROR();

  GLuint vboi;
  glGenBuffers(1,&vboi);                                                      CHECK_GL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi);                                 CHECK_GL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);   CHECK_GL_ERROR();

  t->texture_id = glhelper::load_texture("data/fontB.tga");

  t->visible = true;
  t->prog = gui_program_id;
}

GLuint upload_mesh_to_gpu(const mesh& m)
{
  GLuint vao, vbo, vboi;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1,&vbo);                                 CHECK_GL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER,vbo); CHECK_GL_ERROR();
  glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); CHECK_GL_ERROR();

  glEnableVertexAttribArray(0); CHECK_GL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), 0); CHECK_GL_ERROR();

  glEnableVertexAttribArray(1); CHECK_GL_ERROR();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vertex_opengl), (void*)sizeof(vec3)); CHECK_GL_ERROR();

  glEnableVertexAttribArray(2); CHECK_GL_ERROR();
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(2*sizeof(vec3))); CHECK_GL_ERROR();

  glEnableVertexAttribArray(3); CHECK_GL_ERROR();
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(3*sizeof(vec3))); CHECK_GL_ERROR();

  glGenBuffers(1,&vboi); CHECK_GL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi); CHECK_GL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); CHECK_GL_ERROR();

  return vao;
}



void init_model_2()
{
    mesh m = load_obj_file("data/cube.obj");

  // Affecte une transformation sur les sommets du maillage

  float s = 0.2f;
  float s2 = 0.8f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s2, 0.0f, 0.0f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

   update_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  // Centre la rotation du modele 1 autour de son centre de gravite approximatif
  obj2[1].tr.rotation_center = vec3(-0.2f,-0.4f,0.0f);

   
  //fill_color(&m,vec3(0.5f,0.0f,1.0f));

  obj2[1].vao = upload_mesh_to_gpu(m);

  obj2[1].nb_triangle = m.connectivity.size();
  obj2[1].texture_id = glhelper::load_texture("data/grass.tga");CHECK_GL_ERROR();
  obj2[1].visible = true;
  obj2[1].prog = shader_program_id;

  obj2[1].tr.translation = vec3(2.2f , 0.6f, -5.0);
}


void init_model_3(){
for (int i=0; i<nb_mur; i++){  
  for (int k = 0; k<nb_obj; k++){
    for(int j = 0; j<nb_obj; j++){
    // Chargement d'un maillage a partir d'un fichier
    mesh m = load_obj_file("data/cube.obj");

    // Affecte une transformation sur les sommets du maillage
    float s = 0.2f;
    mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
        0.0f,    s, 0.0f, 0.0f,
        0.0f, 0.0f,   s , 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    apply_deformation(&m,transform);
   
    // Centre la rotation du modele 1 autour de son centre de gravite approximatif
    obj[k][j][i].tr.rotation_center = vec3(0.0f,0.0f,0.0f);

    update_normals(&m);
    fill_color(&m,vec3(1.0f,1.0f,1.0f));

    obj[k][j][i].vao = upload_mesh_to_gpu(m);

   obj[k][j][i].visible = true;
   obj[k][j][i].nb_triangle = m.connectivity.size();
    
   
    obj[k][j][i].texture_id = glhelper::load_texture("data/grass.tga");CHECK_GL_ERROR();

//Création des matrices :
    for (int n = 0; n < nb_matrice; n++) {
        if (n == 0) {
            for (int l = 0; l < nb_obj; l++) {
                for (int m = 0; m < nb_obj; m++) {
                    if (((k <= 3) && (j >= 3) && (j <= 7)))
                    {
                        matrice[n][l][m] = 0;
                    }
                    else {
                        matrice[n][l][m] = 1;
                    }
                }
            }
        }
        if (n == 1) {
            for (int l = 0; l < nb_obj; l++) {
                for (int m = 0; m < nb_obj; m++) {
                    if (((k <= 3) && (j == 5))){
                        matrice[n][l][m] = 0;
                    }
                    else {
                        matrice[n][l][m] = 1;
                    }
                }
            }
        }
        if (n == 2) {
            for (int l = 0; l < nb_obj; l++) {
                for (int m = 0; m < nb_obj; m++) {
                    if (((k <= 3) && (j == 5)) || ((k == 2) && (j >=3) && (j <= 7))) {
                        matrice[n][l][m] = 0;
                    }
                    else {
                        matrice[n][l][m] = 1;
                    }
                }
            }
        }
    }


 if (i==0){
     if (!matrice[i][k][j]) {
         obj[k][j][i].visible = false;
     }
 }
if (i == 1) {
    if (!matrice[i][k][j]) {
        obj[k][j][i].visible = false;
    }
}
if (i == 2) {
    if (!matrice[i][k][j]) {
        obj[k][j][i].visible = false;
    }
}

 // Création des trous à la main.
  /*if (i == 1) {
    if (((k>=3) && (k<=6) &&(j>=3)&&(j<=7))){
       obj[k][j][i].visible = false;
    }
 }

   if (i==2){
    if ((((k==4)||(k==5)||(k==7))&&(j==4))|| ((k>5) && (k<=6) &&(j>=2)&&(j<=6))){
       obj[k][j][i].visible = false;
    }
 }*/


    obj[k][j][i].prog = shader_program_id;

    obj[k][j][i].tr.translation = vec3(0.0 +0.4*j, 0.0+0.4*k, -10-i*7); //avant le z était en -10

}
}
}
}

void init_model_4()
{
    mesh m = load_obj_file("data/cube.obj");

  // Affecte une transformation sur les sommets du maillage

  float s = 0.2f;
  float s2 = 0.4f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s2, 0.0f, 0.0f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

   update_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  // Centre la rotation du modele 1 autour de son centre de gravite approximatif
  obj2[2].tr.rotation_center = vec3(-0.2f,-0.4f,0.0f);

  //fill_color(&m,vec3(0.5f,0.0f,1.0f));

  obj2[2].vao = upload_mesh_to_gpu(m);

  obj2[2].nb_triangle = m.connectivity.size();
  obj2[2].texture_id = glhelper::load_texture("data/container.tga");CHECK_GL_ERROR();
  obj2[2].visible = true;
  obj2[2].prog = shader_program_id;

  obj2[2].tr.translation = vec3(2.6f , 1.4f, -5.0);
}

void init_model_5()
{
    mesh m = load_obj_file("data/cube.obj");

  // Affecte une transformation sur les sommets du maillage

  float s = 0.2f;
  float s2 = 0.4f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s2, 0.0f, 0.0f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

   update_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  // Centre la rotation du modele 1 autour de son centre de gravite approximatif
  obj2[3].tr.rotation_center = vec3(0.2f,-0.4f,0.0f);

  //fill_color(&m,vec3(0.5f,0.0f,1.0f));

  obj2[3].vao = upload_mesh_to_gpu(m);

  obj2[3].nb_triangle = m.connectivity.size();
  obj2[3].texture_id = glhelper::load_texture("data/grass.tga");CHECK_GL_ERROR();
  obj2[3].visible = true;
  obj2[3].prog = shader_program_id;

  obj2[3].tr.translation = vec3(1.8f , 1.4f, -5.0);
}
/*

void init_model_6()
{
    mesh m = load_obj_file("data/cube.obj");

  // Affecte une transformation sur les sommets du maillage

  float s = 30.0f;
  float s2 = 0.002f;
  float s3 = 100.0f;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s2, 0.0f, 0.0f,
      0.0f, 0.0f,   s3 , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,transform);

   update_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  // Centre la rotation du modele 1 autour de son centre de gravite approximatif
  obj2[4].tr.rotation_center = vec3(0.2f,-0.4f,0.0f);

  //fill_color(&m,vec3(0.5f,0.0f,1.0f));

  obj2[4].vao = upload_mesh_to_gpu(m);

  obj2[4].nb_triangle = m.connectivity.size();
  obj2[4].texture_id = glhelper::load_texture("data/route.tga");CHECK_GL_ERROR();
  obj2[4].visible = true;
  obj2[4].prog = shader_program_id;

  obj2[4].tr.translation = obj2[1].tr.translation + vec3(0.0f,-0.6f,0.0f);
}*/
