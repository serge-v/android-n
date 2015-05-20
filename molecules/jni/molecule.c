#include "molecule.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <err.h>
#include "pic.h"
#include "resources.h"

static struct atom *
atom_create(char symbol, int angle, int bond)
{
	struct atom *a = calloc(1, sizeof(struct atom));
	if (a == NULL)
		err(1, "cannot creat atom.");

	a->symbol = symbol;
	a->angle = angle;
	a->bond = bond;

	return a;
}

static struct atom *
create_ethylene()
{
	struct atom *c1, *c2, *h1, *h2, *h3, *h4;

	/*
	 create molecule:

	 H1       H3
	 \      /
	 C1==C2
	 /      \
	 H2       H4
	 */

	c1 = atom_create('C', 0, 0);
	c2 = atom_create('C', 0, 2);
	h1 = atom_create('H', 135, 1);
	h2 = atom_create('H', -135, 1);
	h3 = atom_create('H', 45, 1);
	h4 = atom_create('H', -45, 1);

	c1->child = h1;
	h1->next = h2;
	h2->next = c2;

	c2->child = h3;
	h3->next = h4;

	return c1;
}

static struct atom *
create_ethane()
{
	struct atom *c1, *c2, *h1, *h2, *h3, *h4, *h5, *h6;

	/*
	 create molecule:

	     H1    H4
	      \    /
	  H2--C1--C2--H5
	      /    \
	     H3     H6
	 */

	c1 = atom_create('C', 0, 0);
	c2 = atom_create('C', 0, 1);
	h1 = atom_create('H', 135, 1);
	h2 = atom_create('H', 180, 1);
	h3 = atom_create('H', -135, 1);
	h4 = atom_create('H', 45, 1);
	h5 = atom_create('H', 0, 1);
	h6 = atom_create('H', -45, 1);

	c1->child = c2;
	c2->next = h1;
	h1->next = h2;
	h2->next = h3;

	c2->child = h4;
	h4->next = h5;
	h5->next = h6;

	return c1;
}

static struct atom *
create_methane()
{
	struct atom *c1, *h1, *h2, *h3, *h4;

	/*
	     H1
	     |
	  H4-C1-H3
	     |
	     H2
	 */

	c1 = atom_create('C', 0, 0);
	h1 = atom_create('H', 0, 1);
	h2 = atom_create('H', 180, 1);
	h3 = atom_create('H', 90, 1);
	h4 = atom_create('H', -90, 1);

	c1->child = h1;
	h1->next = h2;
	h2->next = h3;
	h3->next = h4;

	return c1;
}

void
molecule_create(struct molecule* p, const char* name, float x, float y)
{
	memset(p, 0, sizeof(struct molecule));

	p->name = strdup(name);
	p->scale = 1;
	p->x = x;
	p->y = y;

	if (strcmp(name, "ethylene") == 0)
		p->a = create_ethylene();
	else if (strcmp(name, "methane") == 0)
		p->a = create_methane();
	else if (strcmp(name, "ethane") == 0)
		p->a = create_ethane();
	else
		p->a = atom_create('H', 0, 0);
}

static void
atom_draw(const struct atom* atom, const struct atom* parent, bool bonds)
{
	glRotatef(atom->angle, 0, 0, 1);
	glTranslatef(0.75, 0, 0);
	if (parent != NULL && bonds) {
		if (atom->bond == 1)
			pic_draw(&R.bond1_pic);
		else if (atom->bond == 2)
			pic_draw(&R.bond2_pic);
	}
	glTranslatef(0.75, 0, 0);

	if (!bonds) {
		if (atom->symbol == 'H')
			pic_draw2(&R.hidrogen_pic, &R.hidrogen_mask_pic);
		else
			pic_draw2(&R.carbon_pic, &R.hidrogen_mask_pic);
	}

	for (struct atom *a = atom->child; a != NULL; a = a->next)
		atom_draw(a, atom, bonds);

	glTranslatef(-1.5, 0, 0);
	glRotatef(-atom->angle, 0, 0, 1);
}

void
molecule_draw(const struct molecule* p)
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(p->x, p->y, 0);
	atom_draw(p->a, NULL, true);
	glLoadIdentity();
	glTranslatef(p->x, p->y, 0);
	atom_draw(p->a, NULL, false);
	glPopMatrix();
}

static void
atom_destroy(struct atom *atom)
{
	struct atom *tmp;
	for (struct atom *a = atom->child; a != NULL;) {
		tmp = a;
		a = a->next;
		atom_destroy(tmp);
	}
	free(atom);
}

void
molecule_destroy(struct molecule* p)
{
	if (p->name)
		free((char*)p->name);

	atom_destroy(p->a);
	memset(p, 0, sizeof(struct molecule));
}


























