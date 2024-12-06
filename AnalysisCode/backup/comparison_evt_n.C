
#include "Riostream.h"

void comparison_evt_n()
{
   ifstream in1, in2;
   in1.open("./event_n.txt");
   in2.open("./evt_n_sync.txt");

   Int_t x,y;
   Int_t nlines = 0;

   std::vector<Int_t> evtn1;
   std::vector<Int_t> evtn2;

   while (1) 
   {
      in1 >> x;
      if (!in1.good()) break;
      evtn1.push_back(x);
      //printf("x=%i\n",x);
      nlines++;
   }

   nlines = 0;
   while (1)
   {
      in2 >> y;
      if (!in2.good()) break;
      evtn2.push_back(y);
      //printf("y=%i\n",y);
      nlines++;
   }

   in1.close();
   in2.close();

   cout << evtn1.size() << "   " << evtn2.size() << endl;
  
   std::vector<Int_t> unmatched;
   Int_t found = 0;
   for (Int_t i=0;i<evtn1.size();i++)
   {
      for (Int_t j=0;j<evtn2.size();j++)
      {
         if (evtn1.at(i) == evtn2.at(j)) {found++;continue;}
      }
   
      if (found==0){unmatched.push_back(evtn1.at(i));}
   } 

   for (Int_t k=0;k<unmatched.size();k++)
   {
      cout << unmatched.at(k) << endl;
   }

   cout << "Comparison Done!" << endl;
}
