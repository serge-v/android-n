struct sphere {
	float radius;
	int rings;
	int sectors;

	GLfloat* vertices;
	GLfloat* normals;
	GLfloat* texcoords;
	GLushort* indices;

	int vertices_size;
	int normals_size;
	int texcoords_size;
	int indices_size;
};

void create_solid_sphere(struct sphere* sph, float radius, int rings, int sectors);

void create_solid_sphere(struct sphere* sph, float radius, int rings, int sectors)
{
	float const R = 1.0f / (float)(rings - 1);
	float const S = 1.0f / (float)(sectors - 1);
	int r, s;
	GLfloat* v, *n, *t;
	GLushort* i;

	sph->radius = radius;
	sph->rings = rings;
	sph->sectors = sectors;

	sph->vertices_size = rings * sectors * 3;
	sph->vertices = malloc(sph->vertices_size * sizeof(GLfloat));
	sph->normals_size = rings * sectors * 3;
	sph->normals = malloc(sph->normals_size * sizeof(GLfloat));
	sph->texcoords_size = rings * sectors * 2;
	sph->texcoords = malloc(sph->texcoords_size * sizeof(GLfloat));
	sph->indices_size = rings * sectors * 4;
	sph->indices = malloc(sph->indices_size * sizeof(GLushort));

	v = sph->vertices;
	n = sph->normals;
	t = sph->texcoords;
	i = sph->indices;

	for (r = 0; r < rings; r++) {
		for (s = 0; s < sectors; s++) {
			float const y = sinf(-M_PI_2 + M_PI * r * R);
			float const x = cosf(2 * M_PI * s * S) * sinf(M_PI * r * R);
			float const z = sinf(2 * M_PI * s * S) * sinf(M_PI * r * R);

			*t++ = s * S;
			*t++ = r * R;

			*v++ = x * radius;
			*v++ = y * radius;
			*v++ = z * radius;

			*n++ = x;
			*n++ = y;
			*n++ = z;
		}
	}

	for (r = 0; r < rings; r++) {
		for (s = 0; s < sectors; s++) {
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s + 1);
			*i++ = (r + 1) * sectors + (s + 1);
			*i++ = (r + 1) * sectors + s;
		}
	}
}

/*
void draw_sphere(struct sphere* sph, GLfloat x, GLfloat y, GLfloat z)
{
  glMatrixMode(GL_MODELVIEW);

  glTranslatef(x, y, z);

  glEnableClientState(GL_VERTEX_ARRAY);
//  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, sph->vertices);
  LOGGLE("glVertexPointer");

  glNormalPointer(GL_FLOAT, 0, &sph->normals);
  LOGGLE("glNormalPointer");

  glTexCoordPointer(2, GL_FLOAT, 0, &sph->texcoords);
  glDrawElements(GL_QUADS, sph->indices_size / 4, GL_UNSIGNED_SHORT, sph->indices);

  LOGGLE("glDrawElements");
//  LOGI("%d", sph->indices_size);

  glDisableClientState(GL_VERTEX_ARRAY);
//  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
*/
/*
void glutSolidCube( GLfloat dSize )
{
    float size = dSize * 0.5;

#   define V(a,b,c) glVertex3d( a size, b size, c size );
#   define N(a,b,c) glNormal3d( a, b, c );

    glBegin( GL_QUADS );
        N( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+);
        N( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+);
        N( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+);
        N(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-);
        N( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+);
        N( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-);
    glEnd();

#   undef V
#   undef N
}
*/

/*
void draw_cube()
{
    glRotatef(10, 1.0, 0.0, 0.0);
    glRotatef(20, 0.0, 1.0, 0.0);
    glRotatef(30, 0.0, 0.0, 1.0);
    glColor3f(0.4f, 0.0f, 0.0f);
    glutSolidCube(0.5);
}
*/
