/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG
 * spanel@fit.vutbr.cz
 *
 * $Id:$
 */

/* student.c
 * Riesenie IZG-Projekt, 2015/2016
 * Autor: Róbert Kolcún, FIT
 * <xkolcu00@stud.fit.vutbr.cz>
*/

#include "student.h"
#include "transform.h"
#include "fragment.h"

#include <memory.h>
#include <math.h>


/*****************************************************************************
 * Globalni promenne a konstanty
 */

float glob_n = 0;
const S_Material	MAT_WHITE_AMBIENT = { 1.0, 1.0, 1.0, 1.0 };
const S_Material    MAT_WHITE_DIFFUSE = { 1.0, 1.0, 1.0, 1.0 };
const S_Material    MAT_WHITE_SPECULAR = { 1.0, 1.0, 1.0, 1.0 };

/* Typ/ID rendereru (nemenit) */
const int           STUDENT_RENDERER = 1;


/*****************************************************************************
 * Funkce vytvori vas renderer a nainicializuje jej
 */

S_Renderer * studrenCreate()
{
    S_StudentRenderer * renderer = (S_StudentRenderer *)malloc(sizeof(S_StudentRenderer));
    IZG_CHECK(renderer, "Cannot allocate enough memory");

    /* inicializace default rendereru */
    renderer->base.type = STUDENT_RENDERER;
    renInit(&renderer->base);

    /* nastaveni ukazatelu na upravene funkce */
    /* napr. renderer->base.releaseFunc = studrenRelease; */
	renderer->base.releaseFunc = studrenRelease;
	renderer->base.projectTriangleFunc = studrenProjectTriangle;

    /* inicializace nove pridanych casti */
    /* ??? */
	renderer->texturaFarba = loadBitmap(TEXTURE_FILENAME, &renderer->width, &renderer->height);

    return (S_Renderer *)renderer;
}

/*****************************************************************************
 * Funkce korektne zrusi renderer a uvolni pamet
 */

void studrenRelease(S_Renderer **ppRenderer)
{
    S_StudentRenderer * renderer;

    if( ppRenderer && *ppRenderer )
    {
        /* ukazatel na studentsky renderer */
        renderer = (S_StudentRenderer *)(*ppRenderer);

        /* pripadne uvolneni pameti */
        /* ??? */
		free(renderer->texturaFarba);
        
        /* fce default rendereru */
        renRelease(ppRenderer);
    }
}

/******************************************************************************
 * Nova fce pro rasterizaci trojuhelniku s podporou texturovani
 * Upravte tak, aby se trojuhelnik kreslil s texturami
 * (doplnte i potrebne parametry funkce - texturovaci souradnice, ...)
 * v1, v2, v3 - ukazatele na vrcholy trojuhelniku ve 3D pred projekci
 * n1, n2, n3 - ukazatele na normaly ve vrcholech ve 3D pred projekci
 * x1, y1, ... - vrcholy trojuhelniku po projekci do roviny obrazovky
 */

void studrenDrawTriangle(S_Renderer *pRenderer,
                         S_Coords *v1, S_Coords *v2, S_Coords *v3,
                         S_Coords *n1, S_Coords *n2, S_Coords *n3,
                         int x1, int y1,
                         int x2, int y2,
                         int x3, int y3,
						 S_Triangle *triangle,
						 double h1, double h2, double h3
                         )
{
    /* zaklad fce zkopirujte z render.c */
	int         minx, miny, maxx, maxy;
	int         a1, a2, a3, b1, b2, b3, c1, c2, c3;
	int         s1, s2, s3;
	int         x, y, e1, e2, e3;
	double      alpha, beta, gamma, w1, w2, w3, z;
	S_RGBA      col1, col2, col3, color;

	IZG_ASSERT(pRenderer && v1 && v2 && v3 && n1 && n2 && n3);

	/* vypocet barev ve vrcholech */
	col1 = pRenderer->calcReflectanceFunc(pRenderer, v1, n1);
	col2 = pRenderer->calcReflectanceFunc(pRenderer, v2, n2);
	col3 = pRenderer->calcReflectanceFunc(pRenderer, v3, n3);

	/* obalka trojuhleniku */
	minx = MIN(x1, MIN(x2, x3));
	maxx = MAX(x1, MAX(x2, x3));
	miny = MIN(y1, MIN(y2, y3));
	maxy = MAX(y1, MAX(y2, y3));

	/* oriznuti podle rozmeru okna */
	miny = MAX(miny, 0);
	maxy = MIN(maxy, pRenderer->frame_h - 1);
	minx = MAX(minx, 0);
	maxx = MIN(maxx, pRenderer->frame_w - 1);

	/* Pineduv alg. rasterizace troj.
	hranova fce je obecna rovnice primky Ax + By + C = 0
	primku prochazejici body (x1, y1) a (x2, y2) urcime jako
	(y1 - y2)x + (x2 - x1)y + x1y2 - x2y1 = 0 */

	/* normala primek - vektor kolmy k vektoru mezi dvema vrcholy, tedy (-dy, dx) */
	a1 = y1 - y2;
	a2 = y2 - y3;
	a3 = y3 - y1;
	b1 = x2 - x1;
	b2 = x3 - x2;
	b3 = x1 - x3;

	/* koeficient C */
	c1 = x1 * y2 - x2 * y1;
	c2 = x2 * y3 - x3 * y2;
	c3 = x3 * y1 - x1 * y3;

	/* vypocet hranove fce (vzdalenost od primky) pro protejsi body */
	s1 = a1 * x3 + b1 * y3 + c1;
	s2 = a2 * x1 + b2 * y1 + c2;
	s3 = a3 * x2 + b3 * y2 + c3;

	if (!s1 || !s2 || !s3)
	{
		return;
	}

	/* normalizace, aby vzdalenost od primky byla kladna uvnitr trojuhelniku */
	if (s1 < 0)
	{
		a1 *= -1;
		b1 *= -1;
		c1 *= -1;
	}
	if (s2 < 0)
	{
		a2 *= -1;
		b2 *= -1;
		c2 *= -1;
	}
	if (s3 < 0)
	{
		a3 *= -1;
		b3 *= -1;
		c3 *= -1;
	}

	/* koeficienty pro barycentricke souradnice */
	alpha = 1.0 / ABS(s2);
	beta = 1.0 / ABS(s3);
	gamma = 1.0 / ABS(s1);

	/* vyplnovani... */
	for (y = miny; y <= maxy; ++y)
	{
		/* inicilizace hranove fce v bode (minx, y) */
		e1 = a1 * minx + b1 * y + c1;
		e2 = a2 * minx + b2 * y + c2;
		e3 = a3 * minx + b3 * y + c3;

		for (x = minx; x <= maxx; ++x)
		{
			if (e1 >= 0 && e2 >= 0 && e3 >= 0)
			{
				/* interpolace pomoci barycentrickych souradnic
				e1, e2, e3 je aktualni vzdalenost bodu (x, y) od primek */
				w1 = alpha * e2;
				w2 = beta * e3;
				w3 = gamma * e1;

				/* interpolace z-souradnice */
				z = w1 * v1->z + w2 * v2->z + w3 * v3->z;

				/* INTERPOLACIA TEXTUROVACICH SURADNIC */
				double u, v;

				//UPRAVIT VZOREC Hi - homogenna suradnica //Homog sur. Hi vrati funkcia trProjectVertex()
				/*u = w1 * triangle->t[0].x + w2 * triangle->t[1].x + w3 * triangle->t[2].x;
				v = w1 * triangle->t[0].y + w2 * triangle->t[1].y + w3 * triangle->t[2].y;*/

				u = (w1 * (triangle->t[0].x / h1) + w2 * (triangle->t[1].x / h2) + w3 * (triangle->t[2].x / h3)) / (w1 / h1 + w2 / h2 + w3 / h3);
				v = (w1 * (triangle->t[0].y / h1) + w2 * (triangle->t[1].y / h2) + w3 * (triangle->t[2].y / h3)) / (w1 / h1 + w2 / h2 + w3 / h3);

				/* ZISKANIE FARBY "TEXTURY" PO INTERPOLACII */
				S_RGBA farba = studrenTextureValue(pRenderer, u, v);

				/* interpolace barvy */
				color.red = ROUND2BYTE(((w1 * col1.red + w2 * col2.red + w3 * col3.red) / 255) * farba.red);
				color.green = ROUND2BYTE(((w1 * col1.green + w2 * col2.green + w3 * col3.green) / 255) * farba.green);	// / 255 * farba.green);
				color.blue = ROUND2BYTE(((w1 * col1.blue + w2 * col2.blue + w3 * col3.blue) / 255) * farba.blue);		// / 255 * farba.blue);
				color.alpha = 255;

				/* vykresleni bodu */
				if (z < DEPTH(pRenderer, x, y))
				{
					PIXEL(pRenderer, x, y) = color;
					DEPTH(pRenderer, x, y) = z;
				}
			}

			/* hranova fce o pixel vedle */
			e1 += a1;
			e2 += a2;
			e3 += a3;
		}
	}
}

/******************************************************************************
 * Vykresli i-ty trojuhelnik n-teho klicoveho snimku modelu
 * pomoci nove fce studrenDrawTriangle()
 * Pred vykreslenim aplikuje na vrcholy a normaly trojuhelniku
 * aktualne nastavene transformacni matice!
 * Upravte tak, aby se model vykreslil interpolovane dle parametru n
 * (cela cast n udava klicovy snimek, desetinna cast n parametr interpolace
 * mezi snimkem n a n + 1)
 * i - index trojuhelniku
 * n - index klicoveho snimku (float pro pozdejsi interpolaci mezi snimky)
 */

void studrenProjectTriangle(S_Renderer *pRenderer, S_Model *pModel, int i, float n)
{
    /* zaklad fce zkopirujte z render.c */
	S_Coords    aa, bb, cc;             /* souradnice vrcholu po transformaci */
	S_Coords    naa, nbb, ncc;          /* normaly ve vrcholech po transformaci */
	S_Coords    nn;                     /* normala trojuhelniku po transformaci */
	int         u1, v1, u2, v2, u3, v3; /* souradnice vrcholu po projekci do roviny obrazovky */
	S_Triangle  * triangle;
	int         vertexOffset, normalOffset; /* offset pro vrcholy a normalove vektory trojuhelniku */
	int         i0, i1, i2, in;             /* indexy vrcholu a normaly pro i-ty trojuhelnik n-teho snimku */

	IZG_ASSERT(pRenderer && pModel && i >= 0 && i < trivecSize(pModel->triangles) && n >= 0);

	//********************************************
	/* z modelu si vytahneme i-ty trojuhelnik */
	triangle = trivecGetPtr(pModel->triangles, i);

	/* ziskame offset pro vrcholy n-teho snimku */
	vertexOffset = (((int) n) % pModel->frames) * pModel->verticesPerFrame;

	/* ziskame offset pro normaly trojuhelniku n-teho snimku */
	normalOffset = (((int) n) % pModel->frames) * pModel->triangles->size;

	/* indexy vrcholu pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
	i0 = triangle->v[0] + vertexOffset;
	i1 = triangle->v[1] + vertexOffset;
	i2 = triangle->v[2] + vertexOffset;

	/* index normaloveho vektoru pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
	in = triangle->n + normalOffset;

	//*********************************************
	int         vertexOffsetN, normalOffsetN;
	int         i0N, i1N, i2N, inN;

	vertexOffsetN = (((int)(n + 1)) % pModel->frames) * pModel->verticesPerFrame;
	normalOffsetN = (((int)(n + 1)) % pModel->frames) * pModel->triangles->size;

	i0N = triangle->v[0] + vertexOffsetN;
	i1N = triangle->v[1] + vertexOffsetN;
	i2N = triangle->v[2] + vertexOffsetN;

	inN = triangle->n + normalOffsetN;

	S_Coords aaN, bbN, ccN, aaN1, bbN1, ccN1;
	S_Coords nnN;

	aa = *cvecGetPtr(pModel->vertices, i0);
	bb = *cvecGetPtr(pModel->vertices, i1);
	cc = *cvecGetPtr(pModel->vertices, i2);

	aaN1 = *cvecGetPtr(pModel->vertices, i0N);
	bbN1 = *cvecGetPtr(pModel->vertices, i1N);
	ccN1 = *cvecGetPtr(pModel->vertices, i2N);

	float cast = n - ((int) n);

	aaN.x = (aa.x * (1 - cast)) + (aaN1.x * cast);
	aaN.y = (aa.y * (1 - cast)) + (aaN1.y * cast);
	aaN.z = (aa.z * (1 - cast)) + (aaN1.z * cast);

	bbN.x = (bb.x * (1 - cast)) + (bbN1.x * cast);
	bbN.y = (bb.y * (1 - cast)) + (bbN1.y * cast);
	bbN.z = (bb.z * (1 - cast)) + (bbN1.z * cast);

	ccN.x = (cc.x * (1 - cast)) + (ccN1.x * cast);
	ccN.y = (cc.y * (1 - cast)) + (ccN1.y * cast);
	ccN.z = (cc.z * (1 - cast)) + (ccN1.z * cast);

	//******************NORMALS*********************
	naa = *cvecGetPtr(pModel->normals, i0);
	nbb = *cvecGetPtr(pModel->normals, i1);
	ncc = *cvecGetPtr(pModel->normals, i2);

	aaN1 = *cvecGetPtr(pModel->normals, i0N);
	bbN1 = *cvecGetPtr(pModel->normals, i1N);
	ccN1 = *cvecGetPtr(pModel->normals, i2N);

	aaN1.x = (naa.x * (1 - cast)) + (aaN1.x * cast);
	aaN1.y = (naa.y * (1 - cast)) + (aaN1.y * cast);
	aaN1.z = (naa.z * (1 - cast)) + (aaN1.z * cast);

	bbN1.x = (nbb.x * (1 - cast)) + (bbN1.x * cast);
	bbN1.y = (nbb.y * (1 - cast)) + (bbN1.y * cast);
	bbN1.z = (nbb.z * (1 - cast)) + (bbN1.z * cast);

	ccN1.x = (ncc.x * (1 - cast)) + (ccN1.x * cast);
	ccN1.y = (ncc.y * (1 - cast)) + (ccN1.y * cast);
	ccN1.z = (ncc.z * (1 - cast)) + (ccN1.z * cast);

	//**********NORMALA PRE TRANSFORMACIU***********
	nn = *cvecGetPtr(pModel->trinormals, in);
	nnN = *cvecGetPtr(pModel->trinormals, inN);

	nnN.x = (nn.x * (1 - cast)) + (nnN.x * cast);
	nnN.y = (nn.y * (1 - cast)) + (nnN.y * cast);
	nnN.z = (nn.z * (1 - cast)) + (nnN.z * cast);

	//*****************TRANSFORMACIA********************
	/* transformace vrcholu matici model */
	trTransformVertex(&aa, &aaN);//cvecGetPtr(pModel->vertices, i0));
	trTransformVertex(&bb, &bbN);//cvecGetPtr(pModel->vertices, i1));
	trTransformVertex(&cc, &ccN);//cvecGetPtr(pModel->vertices, i2));

	/* promitneme vrcholy trojuhelniku na obrazovku */
	double h1 = trProjectVertex(&u1, &v1, &aa);
	double h2 = trProjectVertex(&u2, &v2, &bb);
	double h3 = trProjectVertex(&u3, &v3, &cc);

	/* pro osvetlovaci model transformujeme take normaly ve vrcholech */
	trTransformVector(&naa, &aaN1);//cvecGetPtr(pModel->normals, i0));
	trTransformVector(&nbb, &bbN1);//cvecGetPtr(pModel->normals, i1));
	trTransformVector(&ncc, &ccN1);//cvecGetPtr(pModel->normals, i2));

	/* normalizace normal */
	coordsNormalize(&naa);
	coordsNormalize(&nbb);
	coordsNormalize(&ncc);

	/* transformace normaly trojuhelniku matici model */
	trTransformVector(&nn, &nnN);//cvecGetPtr(pModel->trinormals, in));

	/* normalizace normaly */
	coordsNormalize(&nn);

	/* je troj. privraceny ke kamere, tudiz viditelny? */
	if (!renCalcVisibility(pRenderer, &aa, &nn))
	{
		/* odvracene troj. vubec nekreslime */
		return;
	}

	/* rasterizace trojuhelniku */
	studrenDrawTriangle(pRenderer,
		&aa, &bb, &cc,
		&naa, &nbb, &ncc,
		u1, v1, u2, v2, u3, v3,
		triangle,
		h1, h2, h3
		);
}

/******************************************************************************
* Vraci hodnotu v aktualne nastavene texture na zadanych
* texturovacich souradnicich u, v
* Pro urceni hodnoty pouziva bilinearni interpolaci
* Pro otestovani vraci ve vychozim stavu barevnou sachovnici dle uv souradnic
* u, v - texturovaci souradnice v intervalu 0..1, ktery odpovida sirce/vysce textury
*/

S_RGBA studrenTextureValue( S_StudentRenderer * pRenderer, double u, double v )
{
	//printf("%f %f \n", u, v);

	//int len = pRenderer->width;
	//u *= len;
	//v *= pRenderer->height;

	//S_RGBA bod1 = pRenderer->texturaFarba[(int) (v * pRenderer->height) + ((int) u)];

	S_RGBA bod1 = pRenderer->texturaFarba[((int)(v * pRenderer->height)) + pRenderer->width * ((int) (u * pRenderer->width))];
	S_RGBA bod2 = pRenderer->texturaFarba[((int)(v * pRenderer->height + 1)) + pRenderer->width * ((int) (u * pRenderer->width))];
	S_RGBA bod3 = pRenderer->texturaFarba[((int)(v * pRenderer->height)) + pRenderer->width * ((int) (u * pRenderer->width) + 1)];
	S_RGBA bod4 = pRenderer->texturaFarba[((int)(v * pRenderer->height + 1)) + pRenderer->width * ((int) (u * pRenderer->width) + 1)];


	/*S_RGBA bod1 = pRenderer->texturaFarba[((int) (v * pRenderer->height))		* (pRenderer->width - 1) +  ((int) (u * pRenderer->width) - 1)];
	S_RGBA bod2 = pRenderer->texturaFarba[((int) (v * pRenderer->height + 1))	* (pRenderer->width - 1) +  ((int) (u * pRenderer->width) - 1)];
	S_RGBA bod3 = pRenderer->texturaFarba[((int) (v * pRenderer->height))		* (pRenderer->width - 1) + (((int) (u * pRenderer->width) - 1) + 1)];
	S_RGBA bod4 = pRenderer->texturaFarba[((int) (v * pRenderer->height + 1))	* (pRenderer->width - 1) + (((int) (u * pRenderer->width) - 1) + 1)];*/

	bod1.red	=	bod1.red	* (1 - u) + bod2.red	* u;
	bod1.green	=	bod1.green	* (1 - u) + bod2.green	* u;
	bod1.blue	=	bod1.blue	* (1 - u) + bod2.blue	* u;

	bod3.red	=	bod3.red	* (1 - u) + bod4.red	* u;
	bod3.green	=	bod3.green	* (1 - u) + bod4.green	* u;
	bod3.blue	=	bod3.blue	* (1 - u) + bod4.blue	* u;

	bod1.red	=	bod1.red	* (1 - v) + bod3.red	* v;
	bod1.green	=	bod1.green	* (1 - v) + bod3.green	* v;
	bod1.blue	=	bod1.blue	* (1 - v) + bod3.blue	* v;

	return bod1;

    /* ??? */
    unsigned char c = ROUND2BYTE( ( ( fmod( u * 10.0, 1.0 ) > 0.5 ) ^ ( fmod( v * 10.0, 1.0 ) < 0.5 ) ) * 255 );
    return makeColor( c, 255 - c, 0 );
}

/******************************************************************************
 ******************************************************************************
 * Funkce pro vyrenderovani sceny, tj. vykresleni modelu
 * Upravte tak, aby se model vykreslil animovane
 * (volani renderModel s aktualizovanym parametrem n)
 */

void renderStudentScene(S_Renderer *pRenderer, S_Model *pModel)
{
    /* zaklad fce zkopirujte z main.c */
	/* test existence frame bufferu a modelu */
	IZG_ASSERT(pModel && pRenderer);

	/* nastavit projekcni matici */
	trProjectionPerspective(pRenderer->camera_dist, pRenderer->frame_w, pRenderer->frame_h);

	/* vycistit model matici */
	trLoadIdentity();

	/* nejprve nastavime posuv cele sceny od/ke kamere */
	trTranslate(0.0, 0.0, pRenderer->scene_move_z);

	/* nejprve nastavime posuv cele sceny v rovine XY */
	trTranslate(pRenderer->scene_move_x, pRenderer->scene_move_y, 0.0);

	/* natoceni cele sceny - jen ve dvou smerech - mys je jen 2D... :( */
	trRotateX(pRenderer->scene_rot_x);
	trRotateY(pRenderer->scene_rot_y);

	/* nastavime material */
	renMatAmbient(pRenderer, &MAT_WHITE_AMBIENT);
	renMatDiffuse(pRenderer, &MAT_WHITE_DIFFUSE);
	renMatSpecular(pRenderer, &MAT_WHITE_SPECULAR);

	/* a vykreslime nas model (ve vychozim stavu kreslime pouze snimek 0) */
	renderModel(pRenderer, pModel, glob_n);
	//*********************************** - Upravit renderModel
}

/* Callback funkce volana pri tiknuti casovace
 * ticks - pocet milisekund od inicializace */
void onTimer( int ticks )
{
    /* uprava parametru pouzivaneho pro vyber klicoveho snimku
     * a pro interpolaci mezi snimky */
	glob_n += 1 / 30.0f;
}

/*****************************************************************************
 *****************************************************************************/
