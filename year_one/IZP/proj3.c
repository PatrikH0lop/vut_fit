 /**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza
 * Complete linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 * 
 * + Patrik Holop, xholop01
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

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

#define ERROR -1

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

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
    c->obj = malloc(sizeof(struct obj_t)*cap);
    if (c->obj == NULL) {
        fprintf(stderr,"Error: mallocating memory\n");
        c->capacity = 0;
    }
    else
		c->capacity = cap;
    c->size = 0;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
    free(c->obj);
    c->capacity = 0;
    c->size = 0;
    c->obj = NULL;
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
    if (c->size < c->capacity) {
        c->obj[c->size]=obj;
    }
    else {
        c = resize_cluster(c, c->size+1);
        c->obj[c->size]=obj;
    }
    (c->size)++;
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

    if (c1->size + c2->size > c1->capacity) {
        c1->obj = realloc(c1->obj,(c1->size+c2->size)*sizeof(struct obj_t));
        c1->capacity = c1->size + c2->size;
    }
    int ind = c1->size;

    int ind2 = 0;
    while (ind<c1->capacity && ind2<c2->size) {
        c1->obj[ind]=c2->obj[ind2];
        c1->size = c1->size + 1;
        ind++;
        ind2++;
    }
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

	//TODO
    clear_cluster(&carr[idx]);
	for (int i = idx; i<narr-1; i++) {
		carr[i]=carr[i+1];
	}
	return narr-1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO
    return sqrt((fabs(o1->x-o2->x))*(fabs(o1->x-o2->x))+(fabs(o1->y-o2->y))*(fabs(o1->y-o2->y)));
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO
    float distance = 0.0;
	for (int i=0; i<c1->size; i++)
		for (int j=0; j<c2->size; j++) {
			float hDistance = obj_distance(&(c1->obj[i]),&(c2->obj[j]));
			if (hDistance > distance)
				distance = hDistance;
			}
	return distance;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO
    if (narr==1) {
        *c1 = 0;
        *c2 = 0;
    }

    int i = 0;
    int j = 1;
    float distance = cluster_distance(&carr[i],&carr[j]);
    float hDistance = 0;
	for (i=0; i<narr-1; i++)
		for (j=i+1; j<narr; j++) {
			if (i!=j) {
				hDistance = cluster_distance(&carr[i],&carr[j]);
				if (hDistance <= distance) {
					*c1 = i;
					*c2 = j;
					distance = hDistance;
				}
			}
		}
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

    FILE *f = NULL;
    f = fopen(filename, "rt");
    if (f==NULL) {
        fprintf(stderr,"Error: opening file\n");
        *arr = NULL;
        return ERROR;
    }

    char count[7];
    int number = 0;
    int loaded_header = fscanf(f,"%6s%d",count,&number);
    if (loaded_header<2 || number<=0 || (strcmp("count=",count)!=0)) {
        fprintf(stderr,"Wrong head of file: count=N\n");
        *arr = NULL;
        fclose(f);
        return ERROR;
    }

    *arr = malloc(number*sizeof(struct cluster_t));
    if (*arr == NULL) {
        fprintf(stderr,"Error: mallocating memory\n");
        fclose(f);
        return ERROR;
    }

    float id = 0, x = 0, y = 0;
    int loaded_num = 0;
    for (int i=0; i<number; i++) {

    	loaded_num = fscanf(f, "%f %f %f", &id, &x, &y);
    	if (loaded_num<3 || x>1000 || y>1000 || x<0 || y<0) {
            fprintf(stderr,"Error: not correct file\n");
            for (int j=0; j<i; j++) {
                clear_cluster(&(*arr)[j]);
            }
            fclose(f);
            return ERROR;
    	}
        
        for (int q=0; q<i; q++) 
            if ((*arr)[q].obj[0].id == id) {
                fprintf(stderr, "Error: 2 equal identifiers\n");
                for (int j=0; j<i; j++) {
                    clear_cluster(&(*arr)[j]);
                }
                fclose(f);
                return ERROR;
            }


    	init_cluster(&(*arr)[i],1);
    	if ((*arr)[i].obj == NULL) {
            for (int j=0; j<i; j++) {
                clear_cluster(&(*arr)[j]);
            }
            fprintf(stderr,"Error: mallocating memory\n");
            fclose(f);
            return ERROR;
    	}

        struct obj_t object;
        object.x = x;
        object.y = y;
        object.id = id;
        append_cluster(&(*arr)[i],object);
        if ((*arr)[i].obj==NULL) {
            fprintf(stderr,"ERROR: appending object\n");
            for (int j=0; j<i; j++) {
                clear_cluster(&(*arr)[j]);
            }
            fclose(f);
            return ERROR;
        }

	}

	if (fclose(f)==EOF) {
        fprintf(stderr,"Error: closing file\n");
        return ERROR;
	}
	return number;
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

    int clusters_number = 0;
	int loaded_clusters = 0;
    clusters = NULL;

	if (argc == 2) {
    	clusters_number = 1;
		loaded_clusters = load_clusters(argv[1], &clusters);
		if (loaded_clusters == ERROR) {
			fprintf(stderr, "Wrong arguments\n");
			return ERROR;
		}
	}
	else if (argc == 3) {
		char *test;
		clusters_number = strtol(argv[2],&test,10);
		if (*test!='\0' || clusters_number<1) {
			fprintf(stderr, "Wrong arguments\n");
			return ERROR;
		}
		loaded_clusters = load_clusters(argv[1], &clusters);
		if (loaded_clusters == ERROR || clusters_number>loaded_clusters) {
            for (int i=0; i<loaded_clusters; i++) {
            clear_cluster(&clusters[i]);
            }
            free(clusters);
			fprintf(stderr, "Wrong arguments\n");
			return ERROR;
		}
	}
	else {
		fprintf(stderr, "Wrong arguments\n");
		return ERROR;
	}

	/*
	 Complete linkage
	*/

	int c1 = 0, c2 = 0;
	while (loaded_clusters!=clusters_number) {
        find_neighbours(clusters, loaded_clusters, &c1, &c2);
        int new_capacity = clusters[c1].size + clusters[c2].size;
        if (resize_cluster(&clusters[c1], new_capacity)==NULL) {
            for (int i=0; i<loaded_clusters; i++) {
                clear_cluster(&clusters[i]);
            }
            free(clusters);
            fprintf(stderr,"ERROR: resizing memory\n");
            return ERROR;
        }
        merge_clusters(&clusters[c1],&clusters[c2]);
        loaded_clusters = remove_cluster(clusters, loaded_clusters, c2);
	}

    print_clusters(clusters,loaded_clusters);
    for (int i=0; i<loaded_clusters; i++) {
        clear_cluster(&clusters[i]);
    }
    free(clusters);
	return 0;

}
