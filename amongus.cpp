/*This project was divided into 3 parts
a) used prims algorithm 
b) used arbitary insertion to solve the travelling salesperson problem
c) found an optimal solution to the TSP in part b)

This project was created for my EECS281 class and was loosly based on the game amoungus.
The spec for the project will be attached. 
*/
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <limits>
#include <iomanip>
#include <math.h>

using namespace std;

class project4
{
    string ml="";
    enum areas {lab, out, dec};
    struct cord
    {
        int x;
        int y; 
        areas a;
    };

    struct prims
    {
        bool kv=false;
        double dv=numeric_limits<double>::infinity();
        uint32_t pv;
    };

    vector <cord>locations;
    vector <prims> prims_vec;
    vector <uint32_t>tsp;
    vector<uint32_t> path;
    vector<uint32_t> best_path;
    vector< vector <double> >distance_mat;
    uint32_t N;
    double weight_a=0.0;
    double weight_b=0.0;
    double best_weight=0.0;
    double weight_c=0.0;
    bool hasl=false;
    bool hasd=false;
    bool haso=false;

    

    public: 
    //this function is used to recieve different commond lines and then call their respective functions 
    void getMode(int argc, char * argv[])
    {
        option long_options[]=
        {
            {"mode", required_argument, nullptr, 'm'},
            {"help", no_argument, nullptr, 'h'},
            { nullptr, 0, nullptr, '\0' }
        };
    
        opterr=false;
        int choice;
        int option_index=0;

        while((choice= getopt_long(argc, argv, "m:h", long_options, 
                &option_index))!=-1)
        {
            if(choice=='h')
            {
                cout<<"HELP MESSAGE\n";
                exit(0);
            }
            else
            if(choice=='m')
            {
                ml=optarg;
                if ( ml!="MST" && ml!="FASTTSP" && ml!="OPTTSP")
                {
                    cerr<<"Error: Wrong mode input\n";
                    exit(1);
                }
                else
                {
                    read_input();
                    if(ml=="MST")
                    {
                        parta_prims();
                        print_prims();
                    }
                    else
                    if(ml=="FASTTSP")
                    {
                        tsp_arbitary();
                        tsp_print();
                    }
                    else
                    if(ml=="OPTTSP")
                    {
                        part_c();
                    }
                }
            }  
            else 
            {
                cerr<<"Unknown command line option\n";
                exit(1);
            }  
        }
    }

    //this function is reading the input and storing the same in their respective data structures
    void read_input()
    {
        string junk;
        cin>>N;
        locations.resize(N);
        for(uint32_t i=0;i<N;i++)
        {
            cin>>locations[i].x;
            cin>>locations[i].y;
            if(locations[i].x<0 && locations[i].y<0)
            {
                locations[i].a=lab;
                hasl=true;
            }
            else
            if(locations[i].x==0 && locations[i].y<0)
            {
                locations[i].a=dec;
                hasd=true;
            }
            else
            if(locations[i].x<0 && locations[i].y==0)
            {
                locations[i].a=dec;
                hasd=true;
            }
            else
            if(locations[i].x==0 && locations[i].y==0)
            {
                locations[i].a=dec;
                hasd=true;
            }
            else
            {
                locations[i].a=out;
                haso=true;
            }
        }
    }

    //this function uses the prims algorithm 
    void parta_prims()
    {
        prims_vec.resize(N);
        if(hasl==true && haso==true && hasd==false)
        {
            cerr<<"Cannot construct MST"<<endl;
            exit(1);
        }
        prims_vec[0].kv= true;
        prims_vec[0].dv=0.0;
        uint32_t prims_target=0;
        for(uint32_t j=1;j<N;j++)
        {
            double min= numeric_limits<double>::infinity();
            uint32_t min_target=0;
            // I do all my steps inside 1 for loop instead of creating 2 different for loops, 
            //this helped me save on time
            for(uint32_t i=0;i<N;i++) 
            {  
                if(prims_vec[i].kv==false)
                {
                    double ans=find_distance(prims_target,i);
                    if(ans<prims_vec[i].dv)
                    {
                        prims_vec[i].dv=ans;
                        prims_vec[i].pv=prims_target;
                    }
                    if(prims_vec[i].dv<min)
                    {
                        min_target=i;
                        min=prims_vec[i].dv;
                    }
                }
            }
            weight_a+=sqrt(mi n);
            prims_vec[min_target].kv=true;
            prims_target=min_target;
        }

        

    }
    //helper finction to find distance between 2 points 
    double find_distance(uint32_t c1, uint32_t c2)
    {
        if(locations[c1].a==lab && locations[c2].a==out)
        {
            return numeric_limits<double>::infinity();
        }
        else 
        if(locations[c1].a==out && locations[c2].a==lab)
        {
            return numeric_limits<double>::infinity();
        }
        else
        { 
            double x1=(static_cast<double>(locations[c1].x-locations[c2].x))*(locations[c1].x-locations[c2].x); 
            double x2=(static_cast<double>(locations[c1].y-locations[c2].y))*(locations[c1].y-locations[c2].y);
            return x1+x2;
        }
        
    }

    //helper printing function for prims algorithm 
    void print_prims()
    {
        cout<<weight_a<<"\n";
        for(uint32_t i=1;i<N;i++)
        {
            if(i<prims_vec[i].pv)
            {
                cout<<i<<" "<<prims_vec[i].pv<<"\n";
            }
            else
            {
                cout<<prims_vec[i].pv<<" "<<i<<"\n";
            }
        }
    }

    //Helper function for part b to find distance 
    double distance_tsp(uint32_t c1,uint32_t c2)
    {
        double x1=(static_cast<double>(locations[c1].x-locations[c2].x))*(locations[c1].x-locations[c2].x); 
        double x2=(static_cast<double>(locations[c1].y-locations[c2].y))*(locations[c1].y-locations[c2].y);
        return (x1+x2);
    }

    // using arbitary insertion to solve the TSP problem
    void tsp_arbitary()
    {
        tsp.push_back(0);
        uint32_t index=0;
        double min1= numeric_limits<double>::infinity();
        for(uint32_t i=1;i<N;i++)
        {
            double ans= distance_tsp(0,i);
            if(ans<min1)
            {
                min1=ans;
                index=i;
            }
        }
        tsp.push_back(index);
        tsp.push_back(0);
        for(uint32_t k=1;k<N;k++)
        {
            if(k==index) 
            {
                continue;
            }
            else
            {
                double min= numeric_limits<double>::infinity();
                uint32_t min_index=0;
                for(uint32_t i=0;i<tsp.size()-1;i++)
                {
                    double ik=sqrt(distance_tsp(tsp[i],k));
                    double jk=sqrt(distance_tsp(tsp[i+1],k));
                    double ij=sqrt(distance_tsp(tsp[i],tsp[i+1]));
                    double ans= ik+jk-ij;
                    if(ans<min)
                    {
                        min=ans;
                        min_index=i+1; //adding 1 to help when I use .insert 
                    }
                }
                tsp.insert(tsp.begin()+min_index,k);
            }
        }
        for(uint32_t i=0;i<tsp.size()-1;i++)
        {
            weight_b+=sqrt(distance_tsp(tsp[i],tsp[i+1]));
        }

    }

    //helper print function for part b TSP
    void tsp_print()
    {
        cout<<weight_b<<endl;
        for(uint32_t i=0;i<tsp.size()-1;i++)
        {
            cout<<tsp[i]<<" ";
        }
    }

    void part_c()
    {
        tsp_arbitary();
        best_weight=weight_b;
        tsp.pop_back();
        best_path=tsp;
        path=best_path;
        distance_mat.resize(N);// creating a distance matrix to not recaclulate the distance each time 
        for(uint32_t i=0;i<N;i++)
        {
            distance_mat[i].resize(N);
            for(uint32_t j=0;j<N;j++) 
            {
                distance_mat[i][j]= sqrt(distance_tsp(i,j));
            }
        }
        genPerms(1);
        partc_print();
    }

    //using branch and bound algorithm to find the optimal solution 
    void genPerms(uint32_t permLength) 
    {
        if (permLength == path.size()) //once we reach the end 
        {
            if((weight_c+distance_mat[0][path[N-1]])<best_weight)
            {
                best_weight=weight_c+distance_mat[0][path[N-1]];
                best_path=path;
                 
            } 
            return;
        } 

        if (!promising_copy2(permLength)) 
        {
            return;
        } // incase not an optimal path to follow

        for (size_t i = permLength; i < path.size(); ++i) 
        {
            swap(path[permLength], path[i]);
            double ans=distance_mat[path[permLength-1]][path[permLength]]; 
            weight_c+=ans;
            genPerms(permLength + 1);
            weight_c=weight_c-distance_mat[path[permLength-1]][path[permLength]];
            swap(path[permLength], path[i]);
        
        }  
    }  // genPerms()

    //helper print function for partc 
    void partc_print()
    {
        cout<<best_weight<<endl;
        for(uint32_t i=0;i<path.size();i++)
        {
            cout<<best_path[i]<<" ";
        }

    }

    //promising function for branch and bound genPerms 
    bool promising_copy2(uint32_t perml)
    {
        //I copied the prims algorithm - bad coding but I did not want to have a non working part a just in case it didnt work 
        weight_a=0;
        vector <prims> primsc;
        primsc.resize(N); 
        for(uint32_t i=0;i<perml;i++)
        {
            primsc[path[i]].kv=true; 
        }
        primsc[path[perml]].kv= true;
        primsc[path[perml]].dv=0.0;
        uint32_t prims_target=path[perml];
        for(uint32_t j=perml+1;j<N;j++)
        {
            double min= numeric_limits<double>::infinity();
            uint32_t min_target=path[perml];
            for(uint32_t i=perml;i<path.size();i++) 
            {  
                if(primsc[path[i]].kv==false)
                {
                    double ans=distance_mat[prims_target][path[i]];
                    if(ans<primsc[path[i]].dv)
                    {
                        primsc[path[i]].dv=ans;
                        primsc[path[i]].pv=prims_target;
                    }
                    if(primsc[path[i]].dv<min)
                    {
                        min_target=path[i];
                        min=primsc[path[i]].dv;
                    }
                }
            }
            weight_a+=min;
            primsc[min_target].kv=true;
            prims_target=min_target;
        }
        // arm1 is trying to find a closed loop from 0 to a point in the MST
        double arm1min= numeric_limits<double>::infinity();
        for(uint32_t i=perml;i<N;i++)
        {
            if(distance_mat[0][path[i]]<arm1min)
            {
                arm1min=distance_mat[0][path[i]];
            }
        }
        //arm2 is trying to connect the last point in our path to the shortest point in the MST 
        double arm2min= numeric_limits<double>::infinity();
        for(uint32_t i=perml;i<N;i++)
        {
            if(distance_mat[path[perml-1]][path[i]]<arm2min)
            {
                arm2min=distance_mat[path[perml-1]][path[i]];
            }
        }
        double ans=weight_a+weight_c+arm1min+arm2min;
        bool promise=false;
        if(ans<best_weight) 
            promise= true;
        else
            promise= false;
        return promise;
    }
};

int main(int argc, char *argv[]) 
{
    std::ios_base::sync_with_stdio(false);
    std::cout << std::setprecision(2); 
    std::cout << std::fixed; 
    project4 obj=project4();
    obj.getMode(argc,argv);
    return 0;
}

