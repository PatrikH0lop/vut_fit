/**
 * @mainpage Project 3 
 * Documentation for the the 3rd project 
 * "Jednoducha shlukova analyza"
 * Complete linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 * @section Structure 
 * @subsection Modules
 * Description of modules and functions
 * @subsection Structures
 * Description of structures
 */

/**
 * 2D object at position [X,Y]
 */
struct obj_t {
    /// identifier of object
    int id;
    /// X cartesian coordinate of object
    float x;
    /// Y cartesian coordinate of object
    float y;
};

/**
 * Object representing cluster of objects
 */
struct cluster_t {
    /// Number of objects in cluster
    int size;
    /// Maximum number of objects in cluster
    int capacity;
    /// Objects contained in cluster
    struct obj_t *obj;
};

/**
 * \defgroup cluster_functions Functions processing clusters
 * Functions to process, change or compare clusters
 * @{
 */

/**
 * Initializes the cluster to given capacity (with allocating memory)
 * without creating any objects
 * @param c Pointer to cluster to be initialized
 * @param cap Capacity, to which should be cluster initialized
 * @pre c!=NULL
 * @pre cap>=0
 * @post Memory is allocated for cap objects. Size of cluster is 0, capacity of cluster = cap. If error occured, capacity is 0.
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * Sets the cluster to empty and removes all objects in cluster
 * @param c Pointer to cluster to be cleared
 * @pre c!=NULL
 * @post Size of cluster = 0
 * @post Capacity of cluster is 0, all objects in cluster are removed and memory freed.
 */
void clear_cluster(struct cluster_t *c);

/** constant representing chunk of the cluster */
extern const int CLUSTER_CHUNK;


/**
 * Resizes the cluster to given capacity, doesn't delete objects in cluster
 * @param c Pointer to cluster to be resized
 * @param new_cap Capacity, to which should be the cluster resized
 * @pre c!=NULL
 * @pre new_cap>0
 * @post Capacity of cluster = new_cap
 * @return Pointer to resized cluster, or NULL if error occured.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * Appends new object to given cluster
 * @param c Pointer to cluster, to which is object appended
 * @param obj Appended object
 * @pre c!=NULL
 * @pre new_cap>=0
 * @pre c->capacity >=0
 * @post If needed, memory is allocated for object, size of cluster is higher of 1, cluster contains appended object.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * Merges 2 clusters by adding all objects of second cluster to first one
 * @param c1 Pointer to cluster, to which are added objects
 * @param c2 Pointer to cluster, whose object are added to first cluster
 * @pre c1!=NULL
 * @pre c2!=NULL
 * @post 1st cluster contains all objects from 1st and 2nd cluster. If needed, memory is allocated.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
@}
*/

/**
 * @defgroup array_functions Functions processing array of clusters
 * Functions to process or change array of clusters
 * @{
 */

/**
 * Removes the cluster from array
 * @param carr Array, from which is the cluster deleted
 * @param narr Number of clusters in array
 * @param idx Index of cluster (to be deleted) in array
 * @pre c1!=NULL
 * @pre c2!=NULL
 * @pre idx<=narr
 * @post Cluster at index idx is deleted, array contains 1 less cluster
 * @return New size of array
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
@}
*/

/**
 * @defgroup object_functions Functions processing object
 * Functions to process, change or compare objects
 * @{
 */

/**
 * Calculates the distance between 2 objects
 * @param o1 Pointer to first object
 * @param o2 Pointer to second object
 * @pre o1!=NULL
 * @pre o2!=NULL
 * @post Distance>=0
 * @return Distance between objects
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
@}
*/

/**
 * @addtogroup cluster_functions
 * @{
 */

/**
 * Calculates the distance between 2 clusters
 * @param c1 Pointer to first cluster
 * @param c2 Pointer to second cluster
 * @pre c1!=NULL
 * @pre c2!=NULL
 * @post Distance>=0
 * @return Distance between clusters
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
@}
*/


/**
 * @addtogroup array_functions
 * @{
 */

/**
 * Finds the 2 most closest clusters in array
 * @param carr Array of clusters
 * @param narr Number of clusters in array
 * @param c1 Pointer to index of the 1st found cluster
 * @param c2 Pointer to index of the 2nd found cluster
 * @pre c1!=NULL
 * @pre c2!=NULL
 * @pre carr!=NULL
 * @pre narr>0
 * @post c1 = index of the 1st found cluster
 * @post c2 = index of the 2nd found cluster
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
@}
*/

/**
 * @addtogroup cluster_functions
 * @{
 */

/**
 * Sorts all objects in cluster by ID (upwardly)
 * @param c Pointer to cluster of objects
 * @pre c!=NULL
 * @post Objects in cluster are sorted upwardly by ID
 */
void sort_cluster(struct cluster_t *c);

/**
 * Prints all objects of the cluster (formatted)
 * @param c Cluster of objects
 * @pre c!=NULL
 * @post All objects in cluster are printed
 */
void print_cluster(struct cluster_t *c);

/**
@}
*/
 
/**
 * @addtogroup array_functions
 * @{
 */

/**
 * Loads objects from input file, each object to 1 clusters
 * @param filename Name of input file
 * @param arr Array of clusters, to which are loaded clusters appended
 * @pre arr!=NULL
 * @pre filename!=NULL
 * @post Number of loaded objects is created, each contained in 1 cluster, arr = NULL if error occured.
 * @return Number of loaded clusters or ERROR if error occured.
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * Prints all objects in all clusters (formatted)
 * @param carr Array of clusters
 * @param narr Number of clusters in array
 * @pre carr!=NULL
 * @pre narr>=0
 * @post All objects are printed at their clusters
 */
void print_clusters(struct cluster_t *carr, int narr);

/**
@}
*/
