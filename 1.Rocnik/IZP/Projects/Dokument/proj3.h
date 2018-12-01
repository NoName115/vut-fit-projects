/** @file console.h
 * @brief Function prototypes for the console driver.
 *
 * Program for an simple cluster analysis
 *
 * @author Robert Kolcun (xkolcu00)
 * @bug No known bugs.
 */ /** \defgroup Structs
 * @{
 */ /** @brief Struct obj_t is used for declaration of object.
 */ struct obj_t {
    int id; /**< ID of the object */
    float x; /**< X object position */
    float y; /**< Y object position */
};
/** @brief Struct cluster_t is used for declaration of cluster
 */ struct cluster_t {
    int size; /**< Size is actual number of objects in the cluster */
    int capacity; /**< Capacity is the maximal number of objects that could be in the cluster */
    struct obj_t *obj; /**< Obj is the pointer at the array of objects */
};
/**
 * @}
 */ /** \defgroup Cluster_Functions
 * @{
 */ /**
 * @brief Function initializes cluster(c) and allocates memory for number of objects equal to capacity(cap)
 *
 * Cluster is pointing at NULL, if capacity is equal to 0
 * @pre c != NULL
 * @pre cap >=0
 * @post Allocation of memory for cluster
 * @param *c pointer to the adress of cluster which function should initialize array of objects
 * @param cap the number of object which function should initialize memory in the cluster
 * @return void
 */ void init_cluster(struct cluster_t *c, int cap); /** @brief Function deletes the cluster(c), free memory  and initializes cluster with 0 capacity
 * @post Empty cluster
 * @param *c pointer to the cluster which funciton should delete
 * @return void
 */ void clear_cluster(struct cluster_t *c); /** @brief Value recommended for reallocation cluster capacity.
 */ extern const int CLUSTER_CHUNK; /** @brief Function resize cluster(c) to the new capacity (new_cap)
 *
 * @pre c
 * @pre c->capacity >= 0
 * @post Resized cluster to the new capacity
 * @post new_cap >= 0
 * @param *c pointer to the cluster which funcion should resize
 * @param new_cap new capacity for cluster c
 * @return Function returns resized cluster with new capacity
 */ struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap); /** @brief Function add a new object at the end of array of objects in cluster
 *
 * @post Object added at the end of the array of objects in cluster
 * @param *c pointer to the cluster
 * @param obj object which is added at the end of the array of objects in cluster
 * @return void
 */ void append_cluster(struct cluster_t *c, struct obj_t obj); /** @brief Function copy and add all objects from c2 to c1 cluster.
 *
 * @pre c1 != NULL
 * @pre c2 != NULL
 * @post All objects from c2 are add to the c1 cluster
 * @param *c1 pointer to the cluster to which are objects copied
 * @param *c2 pointer to the cluster from which are objects copied
 * @return void
 */ void merge_clusters(struct cluster_t *c1, struct cluster_t *c2); /** @brief Function removes cluster from array of clusters
 *
 * @pre idx < narr
 * @pre narr > 0
 * @post Cluster removed from array of clusters
 * @param *carr pointer to the array of clusters
 * @param narr number of clusters in the array of clusters
 * @param idx position of cluster in the array which should be remove
 * @return New number of clusters in the array
 */ int remove_cluster(struct cluster_t *carr, int narr, int idx); /** @brief Function calculate distance between two obejcts
 *
 * @pre o1 != NULL
 * @pre o2 != NULL
 * @post Distance between two objects
 * @param *o1 pointer to the fist object
 * @param *o2 pointer to the second object
 * @return float value of distance between two objects
 *
 */ /**
 * @}
 */ /** \defgroup Object_Functions
 * @{
 */ float obj_distance(struct obj_t *o1, struct obj_t *o2); /** @brief Function calculates distance between two clusters
 *
 * @pre c1 != NULL
 * @pre c1->size > 0
 * @pre c2 != NULL
 * @pre c2->size > 0
 * @post distance between two clusters
 * @param *c1 pointer to the fist cluster
 * @param *c2 pointer to the second cluster
 * @return float value of distance between two clusters
 */ /**
 * @}
 */ float cluster_distance(struct cluster_t *c1, struct cluster_t *c2); /** @brief Function finds two closest clusters in array of clusters
 *
 * @pre narr > 0
 * @post Indexes of two closest clusters
 * @param *carr is pointer to the array of clusters
 * @param narr number of clusters in the array of clusters
 * @param *c1 pointer to the adress in memory where first cluster is saved
 * @param *c2 pointer to the adress in memory where second cluster is saved
 * @return void
 */ void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2); /** @brief Function sorts all objects in cluster
 *
 * @post Sorted cluster c
 * @param *c pointer to the cluster which should be sorted
 * @return void
 */ void sort_cluster(struct cluster_t *c); /** @brief Functions prints all objects in cluster
 *
 * @pre c != NULL
 * @post Printed objects in the cluster
 * @param *c pointer to the cluster
 * @return void
 */ void print_cluster(struct cluster_t *c); /** @brief Function read objects from the file and for everyone create unique cluster
 *
 * If the function cant read the object from file or other error appear, pointer arr is pointing to NULL
 *
 * @pre arr != NULL
 * @post Returnig number of clusters created and saved to the array of clusters (arr)
 * @param *filename pointer to the name of the file that function should read from
 * @param **arr pointer to the array of clusters
 * @return number of clusters created
 */ int load_clusters(char *filename, struct cluster_t **arr); /** @brief Function print narr clusters from the array of clusters
 *
 * @pre carr = NULL
 * @pre narr > 0
 * @post Printed narr clusters from the array of clusters
 * @param *carr pointer to the array fo clusters
 * @param narr number of clusters that should be printed
 * @return void
 */void print_clusters(struct cluster_t *carr, int narr);
