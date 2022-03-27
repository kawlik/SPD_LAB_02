#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#define COST_MAX 999999

using namespace std;


/*  Declarations
/*   *   *   *   *   *   *   *   *   *   */

typedef int cost_t;
typedef int perm_t;

typedef struct {

    int p;  //  Cost to finish
    int w;  //  weight
    int d;  //  desired end Cost
    int x;  //  id

} task_t;

void readTill( string fileHeader, ifstream &data, vector<task_t> &tasks );

cost_t getCost( task_t &task, int time );

cost_t witi( vector<task_t> &tasks );



/*  Executor
/*   *   *   *   *   *   *   *   *   *   */

int main( int argc, char **argv ) {

    //  initial dataset
    vector<task_t> tasks;
    ifstream data( "witi.data.txt" );
    cost_t totalCost = 0;

    //  process for all 4 datasets
    for ( int i = 10; i <= 20; i++ ) {

        //  reset tasks
        tasks.clear();

        //  create new file header
        string fileHeader = "data." + to_string( i ) + ":";

        //  read data
        readTill( fileHeader, data, tasks );

        //  get partial result
        cost_t result = witi( tasks );

        //  print result
        cout << endl;
        cout << "---+---+---+---+---+---+---+---+---+---" << endl;

        cout << "Current run: " << i << endl;
        cout << "Calculated cost: " << result << endl;
        cout << "Order:" << endl;
        for( int i = 0; i < tasks.size(); i++ ) {
            cout << i + 1 << ". [ r: " << tasks[i].p << " | p: " << tasks[i].w << " | q: " << tasks[i].d << " | x: " << tasks[i].x << "]" << endl;
        }

        cout << "---+---+---+---+---+---+---+---+---+---" << endl;
        cout << endl;

        //  update total result
        totalCost += result;
    }

    //  print result
    cout << endl;
    cout << "---+---+---+---+---+---+---+---+---+---" << endl;

    cout << "<*> Final result <*>" << endl;
    cout << "Calculated total cost: " << totalCost << endl;

    cout << "---+---+---+---+---+---+---+---+---+---" << endl;
    cout << endl;

    //  close data stream
    data.close();

    //  final statement
    return 0;
}


/*  Solutions
/*   *   *   *   *   *   *   *   *   *   */

void readTill( string fileHeader, ifstream &data, vector<task_t> &tasks ) {

    //  initial dataset
    string buffer;
    int size;

    //  read till heder found
    do {
        data >> buffer;
    } while( buffer != fileHeader );

    //  read size
    data >> size;

    //  read data
    for( int  i = 0; i < size; i++ ) {

        //  task buffer
        task_t tmp;

        //  read data into task
        data >> tmp.p >> tmp.w >> tmp.d;

        //  set task id
        tmp.x = i + 1;

        //  add task to list
        tasks.push_back( tmp );
    }
}

cost_t getCost( task_t &task, int time ) {
    return max( 0, task.w * ( time - task.d ));
}

cost_t witi( vector<task_t> &tasks ) {

    //  set numubmer of possible combinations
    unsigned long long num_combinations = ( 1 << tasks.size() );

    //  costs vectors with initial value
    vector<cost_t> costs( num_combinations, COST_MAX );
    vector<perm_t> perms( num_combinations, 1 );
    costs[0] = 0;


    //  loop for all possibilities
    for( int i = 1; i < num_combinations; i++ ) {

        //  start time counter
        int time = 0;

        //  update time for current run
        for( int j = 0; j < tasks.size(); j++ ) {
            if( i & ( 1 << j )) {
                time += tasks[j].p;
            }
        }

        //  find lowest cost
        for( int j = 0; j < tasks.size(); j++ ) {
            if( i && ( 1 << j )) {

                //  calc current cost
                cost_t c = costs[ i & (~( 1 << j )) ] + getCost( tasks[j], time );

                //  check and update cost
                if( costs[i] > c ) {

                    //  update cost
                    costs[i] = c;

                    //  update perm
                    perms[i] = ( 1 << j );
                }
            }
        }
    }

    //  temporaty perm value
    perm_t perm = num_combinations;
    perm_t orders[ tasks.size() ];

    //  find permutation
    for( int i = tasks.size(); i > 0; i-- ) {

        //  parse perm
        perm_t task = perms[perm - 2];

        //  add task to order
        orders[i - 1] = ( perm_t )( log( task ) / log( 2 )) + 1;

        //  update perm
        perm -= task;
    }

    //  print for test
    for( int i = 0; i < tasks.size(); i++ ) {

        //  parse indexes
        int p = orders[i];
        int q = tasks[i].x;

        //  item is on place
        if( p == q ) continue;

        //  find item to swap
        for( int j = i + 1; j < tasks.size(); j++ ) {

            //  check if is the right task
            if( p == tasks[j].x ) {

                //  swap taks
                swap( tasks[i], tasks[j] );

                //  break current loop
                break;
            }
        }
    }
    
    //  return final cost
    return costs.back();
}