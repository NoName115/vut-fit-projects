/**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/slsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct objt_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

/***************************
 * Robert Kolcun, xkolcu00 *
***************************/

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO
    c->capacity = cap;

    if (cap == 0)
    {
        c->size = 0;
        c->obj = NULL;
    }
    else
    {
        c->obj = malloc(cap*sizeof(struct obj_t));
        if (c == NULL) {
            c->capacity = -1;
            return;
        }
        for (int i = 0; i < cap; i++) {
            c->obj[i].id = 0;
            c->obj[i].x = 0;
            c->obj[i].y = 0;
        }
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
    free(c->obj);
    c->obj = NULL;
    c->size = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO
    if (c->size == c->capacity) {
        c = resize_cluster(c, c->capacity + CLUSTER_CHUNK);
        if (c == NULL)
            return;
    }
    c->size++;
    c->obj[c->size - 1] = obj;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
    //Rozsirit zhluk
    if ((c1->size + c2->size) > c1->capacity) {
        c1 = resize_cluster(c1, c1->size + c2->size);
        if (c1 == NULL)
            return;
    }

    //Pridanie objektov z C2 do C1
    for (int i = 0; i < c2->size; i++) {
        append_cluster(c1, c2->obj[i]);
    }

    //V zhluku C1 vzostupne utriedit objekty podla ID
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO
    //Uvolnit pamat
    clear_cluster(&carr[idx]);

    //Posunut vsetky zhluky o 1 od zhluku[idx]
    for (int i = idx; i < narr - 1; i++) {
        carr[i] = carr[i+1];
    }

    //Vrati velkost ArrayOfClusters o 1 mensie
    narr--;
    return narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO
    float xRozdiel = o1->x - o2->x;
    float yRozdiel = o1->y - o2->y;
    float distance = sqrtf(xRozdiel*xRozdiel + yRozdiel*yRozdiel);

    return distance;
}

/*
 Pocita vzdalenost dvou shluku. Vzdalenost je vypoctena na zaklade nejblizsiho
 souseda.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO
    float minimum;
    float totalMinimum = 0;

    for (int i = 0; i < c1->size; i++) {
        for (int j = 0; j < c2->size; j++) {
            minimum = obj_distance(&c1->obj[i], &c2->obj[j]);
            if (totalMinimum == 0 || totalMinimum > minimum) {
                totalMinimum = minimum;
            }
        }
    }

    return totalMinimum;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky (podle nejblizsiho souseda). Nalezene shluky
 identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do
 pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO
    int idx1, idx2;
    float minimum;
    float totalMinimum = 0;

    for (int i = 0; i < narr - 1; i++) {
        for (int j = i + 1; j < narr; j++) {
            minimum = cluster_distance(&carr[i], &carr[j]);
            if (totalMinimum == 0 || totalMinimum > minimum) {
                idx1 = i;
                idx2 = j;
                totalMinimum = minimum;
            }
        }
    }

    *c1 = idx1;
    *c2 = idx2;
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
        //printf("%d", c->size);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO
    FILE *subor;
    subor = fopen(filename, "r");

    if (subor == NULL) {
        *arr = NULL;
        return -1;
    }

    //Inicializuje N a nacita COUNT zo suboru
    int n;
    int chyba;
    chyba = fscanf(subor, "count=%d", &n);
    if (n == 0 || chyba != 1) {
        *arr = NULL;
        return -1;
    }

    //Alokuje miesto pre poleClusterov
    struct cluster_t *arrayOfClusters = malloc(n*sizeof(struct cluster_t)); //struct cluster_t arrayOfClusters[20];
    if (arrayOfClusters == NULL) {
        *arr = NULL;
        return -1;
    }

    //Nacita ID,X,Y daneho bodu a ulozi ho do pamate
    for (int i = 0; i < n; i++)
    {
        init_cluster(&arrayOfClusters[i], CLUSTER_CHUNK);
        arrayOfClusters[i].size = 1;

        if (arrayOfClusters[i].capacity == -1) {
            *arr = NULL;
            return -1;
        }

        chyba = fscanf(subor, "%d%g%g", &arrayOfClusters[i].obj[0].id, &arrayOfClusters[i].obj[0].x, &arrayOfClusters[i].obj[0].y);
        if (chyba != 3) {
            *arr = NULL;
            return -1;
        }
	if (arrayOfClusters[i].obj[0].x < 0 || arrayOfClusters[i].obj[0].x > 1000 || arrayOfClusters[i].obj[0].y < 0 || arrayOfClusters[i].obj[0].y > 1000) {
	  *arr = NULL;
	  return -1;
	}
    }

    fclose(subor);

    *arr = &arrayOfClusters[0];

    //printf("%p\n", *arr);
    //printf("%p\n", &arrayOfClusters[0]);    //nazov[0] == *(nazov + x); - hodnota
    //printf("%p\n", arrayOfClusters);        //&nazov[0] == nazov;       - adresa

    return n;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;

    // TODO
    if (argc == 2 || argc == 3) {
        int sizeOfClusters = 0;
        sizeOfClusters = load_clusters(argv[1], &clusters);
        if (sizeOfClusters == -1 || clusters == NULL) {
            fprintf(stderr, "ERROR: Invalid Input\n");
            return -1;
        }

        int countFinalClusters = 0;
	if (argc == 3) {
          if (sscanf(argv[2], "%d", &countFinalClusters) == 0) {
            fprintf(stderr, "ERROR: Invalid Input\n");
            return -1;
          }
	} else {
	  countFinalClusters = 1;
	}

        if (countFinalClusters <= 0 || countFinalClusters > sizeOfClusters) {
            fprintf(stderr, "ERROR: Invalid Input\n");
            return -1;
        }

        int idx1Sused, idx2Sused;

        while (sizeOfClusters != countFinalClusters) {
            find_neighbours(clusters, sizeOfClusters, &idx1Sused, &idx2Sused);
            merge_clusters(&clusters[idx1Sused], &clusters[idx2Sused]);
            if (&clusters[idx1Sused] == NULL) {
                fprintf(stderr, "ERROR: cluster == NULL\n");
                return -1;
            }
            sizeOfClusters = remove_cluster(clusters, sizeOfClusters, idx2Sused);
        }

        print_clusters(clusters, countFinalClusters);

	for (int i = 0; i < sizeOfClusters; i++) {
	  clear_cluster(&clusters[i]);
	}
	free(clusters);
    } else {
       fprintf(stderr, "ERROR: Invalid number of arguments");
       return -1;
    }

    return 0;
}

