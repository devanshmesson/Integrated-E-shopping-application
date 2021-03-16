#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include<unordered_map>
#include<vector>
#include <algorithm>
#include<cmath>
#include<bits/stdc++.h>
using namespace std;

struct movie_rate{
  int movie_id;
  float rating;
};

bool compare_movie (const movie_rate& lhs,const movie_rate& rhs){
  return lhs.rating>rhs.rating;
}

unordered_map<int, unordered_map<int, float>> rating_map;
unordered_map<int, vector<int>> movie_user;
unordered_map<int, float> avg_rating;
unordered_map<int, string> movie_map;

unordered_map<int, vector<movie_rate>> cache;

bool read_movietitle(string filename) 
{
  freopen(filename.c_str(),"r",stdin);
  //cout<<"hello"<<endl;
  int id_int;
  string ID,  title;///year,
  //return 0;
  while (1)
  {
  
    cin>>id_int;    
    if(id_int==-1)break;
    getline(cin,title);
    movie_map[id_int] = title;
  }
  return true;
}

bool read_rating(string filename)
 {
    freopen(filename.c_str(),"r",stdin);
    int user_id, movie_id;
    float rating;
    string movie_id_str, user_id_str, rating_str;
    while (1)
    {
      cin>>movie_id>>user_id>>rating;
      if(movie_id==-1){break;}
      rating_map[user_id][movie_id]=rating;
      movie_user[movie_id].push_back(user_id);
    }

  return true;
}
void user_rating_avg()
{
  for (auto& user_it : rating_map)
  {
    float sum=0;
    int count = 0;
    for (auto& user_movie_it : user_it.second)
    {
      sum +=user_movie_it.second;
      ++count;
    }
    avg_rating[user_it.first]=sum/count;
  }
}

unordered_map<int, float> get_correlation(int active_user)///function to give correlation values
{
  unordered_map<int, float>corr_map;


  unordered_map<int, float>& active_user_movie_rate=rating_map[active_user];///this unordered map will store the active user movie


int count=0;
 



  for(auto& user_it: rating_map)
        {
    int user_id=user_it.first;

    if(user_id==active_user)
      continue;

    unordered_map<int, float>& user_movie_rate=user_it.second;
    //iterator it;
    /*
    for(auto i:user_movie_rate)
        {
         cout<<"user movie rate unordered map "<<endl;
          cout<<"["<<i.first<<"[ "<<i.second<<endl;

        }
        */

    float nominator = 0;
    float sum_vaj_diff = 0;
    float sum_vij_diff = 0;
         int count=0;
    for(auto& movie_it: active_user_movie_rate)
        {
      int movie_id = movie_it.first;
           /// count=0;
      if(user_movie_rate.find(movie_id) == user_movie_rate.end())
         continue;


      nominator +=  (rating_map[active_user][movie_id] - avg_rating[active_user]) * (rating_map[user_id][movie_id] - avg_rating[user_id]);
      //cout<<"the value of nominator "<<nominator<<endl;
      sum_vaj_diff +=  pow(rating_map[active_user][movie_id] - avg_rating[active_user], 2);
        //     cout<<"the value of vaj_diff "<<sum_vaj_diff<<endl;
      sum_vij_diff +=  pow(rating_map[user_id][movie_id] - avg_rating[user_id], 2);
          // cout<<"the value sum_vij_diff "<<sum_vij_diff<<endl;
           count++;
            //cout<<"the no of times it will run"<<count<<endl;
           //cout<<"---------------------------------------------------------"<<endl;
    }

        float denominator= sqrt(sum_vaj_diff * sum_vij_diff);
        if(denominator!=0)
          corr_map[user_id] = nominator/denominator;
  }
  /*
  for(auto&q:corr_map)
    {

        cout<<"["<<q.first<<"[->"<<q.second<<endl;
    }*/
///cout<<corr_map[1990901]<<endl;
  return corr_map;
}


void movie_recommendation(int active_user, int K)
{

  vector<movie_rate>* predicted_rating=NULL;
    float pred_rating = 0;
    if(cache.find(active_user)==cache.end())
        {

    unordered_map<int, float> corr_active = get_correlation(active_user);
    /*
    for(auto&q:corr_active)
        {

            cout<<"["<<q.first<<q.second<<endl;
        }
        */
    predicted_rating=new vector<movie_rate>();

        //predicted_rating[0].movie_id;
        //predicted_rating[0].rating;

    for(auto& movie_it : movie_map)
            {
                pred_rating=0;


      int movie_id=movie_it.first;

      if(movie_user.find(movie_id)!=movie_user.end())
                {

        vector<int>& user_list=movie_user[movie_id];
        /*
        for(int i=0; i<user_list.size(); i++)
                {

                    cout << user_list.at(i) << ' ';

                }*/

        for(int user_id : user_list)
                    {

          if(corr_active.find(user_id) == corr_active.end())///if search not found that it starts from beginning
            continue;

          pred_rating +=corr_active[user_id]*(rating_map[user_id][movie_id]- avg_rating[user_id]);
          ///cout<<corr_active[user_id];
          ///cout<<rating_map[user_id][movie_id]- avg_rating[user_id];
          ///cout<<pred_rating<<" ";
        }

      }



      movie_rate m;
      m.movie_id=movie_id;
      m.rating=pred_rating;

      predicted_rating->push_back(m);

    }
    sort(predicted_rating->begin(), predicted_rating->end(),compare_movie);

    cache[active_user]=*predicted_rating;

    corr_active.clear();
}
  else
    {
    predicted_rating=&cache[active_user];
  }

  cout<<"\n"<<K <<" recommended movies for user : "<<active_user<<"\n";

  for(int i=0;i<K;++i)
    {
        cout<<movie_map[predicted_rating->at(i).movie_id]<<"->" <<predicted_rating->at(i).rating<<"\n";

    }


}

int main()
{
  string movie_title_file="Mobiles_list.txt";
  string rating_file="ratings_without_reviews.txt";

  cout<< "1.Reading file \""<<movie_title_file<<"\" ...\n\n\n";
  read_movietitle(movie_title_file);
  cout<<movie_title_file<<" "<<"processed successfully!"<<endl;


  cout<<"\n2.Reading file \""<<rating_file<<"\" ...\n\n\n";
  read_rating(rating_file);
  cout<<rating_file<<" "<<"processed successfully!"<<endl;
  
  ifstream fin;
  fin.open("input.txt");
  freopen("output9.txt","w",stdout);
  cout<<"\nComputing User average rating.... \n";
  user_rating_avg();
  int user_id=-2, K;
  while(true)
  {
    cout<<"Please Enter UserID (or enter -1 to exit): "; 
    fin>>user_id; 

    if(user_id==-1)
    {
      break;
    }
    if(rating_map.find(user_id)==rating_map.end())
    {
      cout<<"UserID not found, Please enter valid UserID\n";
    }
    else
      {
      cout<<"Please Enter Number of Recommendation: ";
      fin>>K;
      if(K>0)
      {
        cout<<"Calling movie_recommendation(user_id, K)"<<endl;
        movie_recommendation(user_id, K);
      }
      else
      {
        cout<<"Number of recommendation must be greater than 0\n";
      }
       }
    cout<<"\n";
  }   
  return 0;
}
