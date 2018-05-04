#include "simdag/simdag.h" // MANDATORY
#include "xbt.h" // For dynamic arrays

/* External sources : 
 * - https://github.com/sbadia/simgrid/blob/master/src/xbt/dynar.c 
 */
 
int main(int argc, char **argv) {
  SD_task_t c1, c2, c3, t1, tmp; // 3 tâches séquentielles
  unsigned int ctr;

  // Dynamic array : args = elm_size, free_method
  xbt_dynar_t tasks = xbt_dynar_new(sizeof(SD_task_t), &xbt_free);

  // Initialize SimDag stuff
  SD_init(&argc, argv);

  /* SEQUENTIAL COMPUTATION */
  // Création des tâches : nom, data, flops computed by task
  c1 = SD_task_create_comp_seq("c1", NULL, 1E9);
  c2 = SD_task_create_comp_seq("c2", NULL, 5E9);
  c3 = SD_task_create_comp_seq("c3", NULL, 2E9);
  
  // End-to-end communications : both source and destination are sequential tasks ARGS = name, data, bytes sent
  t1 = SD_task_create_comm_e2e("t1", NULL, 5e8);
  
  /* 
   * Data flow dependency
   *
   * Example 1st : Force SimDag to wait completion of c1
   * before t1
   */
   
  SD_task_dependency_add ("c1-t1", NULL, c1, t1);
  SD_task_dependency_add ("t1-c3", NULL, t1, c3);
  SD_task_dependency_add ("c2-c3", NULL, c2, c3);

  // Add an element at the end of the dynar
  xbt_dynar_push(tasks, &c1); 
  xbt_dynar_push(tasks, &c2);
  xbt_dynar_push(tasks, &c3); 
  xbt_dynar_push(tasks, &t1);

  // Loop over elements in the array
  xbt_dynar_foreach(tasks, ctr, tmp){
    // Retrieve all information on task
    SD_task_dump(tmp); 
    // Destructor
    SD_task_destroy(tmp);
  }
  // free dynar mais pas its content
  xbt_dynar_free_container(&tasks);
  
  // Cleaning SimDag stuff
  SD_exit();
  return 0;
}
