#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>     //open,creat
#include <sys/types.h> //open
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h> //perror, errno

/*-------------------------------------------TODO---------------------------------------------*/
/**TODO:
 * Implement realloc usage          50% (need to actually use it)
 * Wanted worker count function     100%
 * Logic impelementation            100%
 * File Writeing                    100%
 * File Reading                     100%   (needs fixing, is shit)
 * Delete record                    0%
 * Modify record                    0%
 * Cleaning up reappearing code     after it is done
 * Input checks                     should check it (always trying to implement at first try)
 * /

/*-------------------------------------------Structs---------------------------------------------*/
struct worker
{
   int id;
   char *name;
   bool *workdays;
};

int workerWantedHeadcount[7] = {0};
int workerActualHeadcount[7] = {0};

int reallocExtra = 20;

/*-------------------------------------------Functions-------------------------------------------*/
void ReadInWorkerHeadcount(void);
void WorkerDataFree(struct worker *wData, int size);
struct worker *MallocWorkerData(int initialSize);
struct worker *ReallocWorkerData(struct worker *previousWorkerData, int newSize, int oldSize);
void PrintWorker(struct worker *wd, int id);
char *ReadConsoleInput(void);
void PrintWorkerHeadcount(void);
void PrintWorkerLoggedHeadcount(void);
void SaveWorkerData(struct worker *wd, int wdSize);
struct worker *LoadWorkerData(int *actWorkerCount);
int GetMenuChoice(int from, int to);
void DeleteRecord(int idToDelete, struct worker *wd, int* wdSize);
void ModifyRecord(int idToModify, struct worker *wd, int *wdSize);
/*-----------------------------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
   char opt = '\0';
   int workerDataSize = 2;
   int workerCount = 0;

   struct worker *workerData = MallocWorkerData(workerDataSize);

   bool preparationDone = false;
   
   printf("\nKérem válasszon az alábbi menüpontokból!\n[1] Várt munkások számának megadása\n[2] Munkások betöltése file-ból\n");

   int choice = GetMenuChoice(1,2);

   switch (choice)
   {
   case 1:
      ReadInWorkerHeadcount();
      break;

   case 2:
      WorkerDataFree(workerData, workerDataSize);
      workerData = LoadWorkerData(&workerCount);
      break;
      
   default:
      break;
   }


   do
   {
      printf("\nKérem válassza ki a végrehajtandó műveletet az alábbiak közül!\n[1] Új munkás felvétele\n[2] Adatok mentése\n[3] Adatok betöltése\n[4] Adat módosítása\n[5] Adat törlése\n[6] Adatok kiírása\n[7] Kilépés (A nem mentett adatok elvesznek)\n");
      choice = GetMenuChoice(1,7);
      switch (choice)
      {

      case 1:
         printf("\nKérem adja meg a nevét, illetve a napokat amikor munkára jelentkezne!\n");
         printf("\n");
         printf("Várt munkások száma (hétfő-vasárnapi sorrend)");
         PrintWorkerHeadcount();
         printf("\n");
         printf("Betöltött helyek száma (hétfő-vasárnapi sorrend)");
         PrintWorkerLoggedHeadcount();
         printf("\n");

         char *inputString = ReadConsoleInput();

         printf("\n%s\n", inputString);
         printf("Sor hossz: %ld\n", strlen(inputString));
         const char delim[2] = " ";
         char *t;

         t = strtok(inputString, delim);
         bool first_value = true;
         bool redo_needed = false;

         while (t != NULL)
         {
            printf("\n%s", t);

            if (first_value)
            {
               workerData[workerCount].id = workerCount;
               strcpy(workerData[workerCount].name, t);
               first_value = false;
            }
            else if (!redo_needed)
            {

               if (strcmp(t, "hétfő") == 0)
               {
                  if (workerActualHeadcount[0] < workerWantedHeadcount[0])
                  {
                     workerData[workerCount].workdays[0] = true;
                     ++workerActualHeadcount[0];
                  }
                  else
                  {
                     redo_needed = true;
                     printf("\nA hétfői napra várt létszám megtelt");
                  } // Kiíráshoz \n kell többi helyre is / kiegészíteni mindet
               }
               else if (strcmp(t, "kedd") == 0)
               {
                  if (workerActualHeadcount[1] < workerWantedHeadcount[1])
                  {
                     workerData[workerCount].workdays[1] = true;
                     ++workerActualHeadcount[1];
                  }
                  else
                  {
                     redo_needed = true;
                     printf("\nA keddi napra várt létszám megtelt");
                  }
               }
               else if (strcmp(t, "szerda") == 0)
               {
                  if (workerActualHeadcount[2] < workerWantedHeadcount[2])
                  {
                     workerData[workerCount].workdays[2] = true;
                     ++workerActualHeadcount[2];
                  }
                  else
                  {
                     redo_needed = true;
                     printf("\nA szerdai napra várt létszám megtelt");
                  }
               }
               else if (strcmp(t, "csütörtök") == 0)
               {
                  if (workerActualHeadcount[3] < workerWantedHeadcount[3])
                  {
                     workerData[workerCount].workdays[3] = true;
                     ++workerActualHeadcount[3];
                  }
                  else
                  {
                     redo_needed = true;
                     printf("\nA csütörtöki napra várt létszám megtelt");
                  }
               }
               else if (strcmp(t, "péntek") == 0)
               {
                  if (workerActualHeadcount[4] < workerWantedHeadcount[4])
                  {
                     workerData[workerCount].workdays[4] = true;
                     ++workerActualHeadcount[4];
                  }
                  else
                  {
                     redo_needed = true;
                     printf("\nA pénteki napra várt létszám megtelt");
                  }
               }
               else if (strcmp(t, "szombat") == 0)
               {
                  if (workerActualHeadcount[5] < workerWantedHeadcount[5])
                  {
                     workerData[workerCount].workdays[5] = true;
                     ++workerActualHeadcount[5];
                  }
                  else
                  {
                     redo_needed = true;
                     printf("\nA szombati napra várt létszám megtelt");
                  }
               }
               else if (strcmp(t, "vasárnap") == 0)
               {
                  if (workerActualHeadcount[6] < workerWantedHeadcount[6])
                  {
                     workerData[workerCount].workdays[6] = true;
                     ++workerActualHeadcount[6];
                  }
                  else
                  {
                     redo_needed = true;
                     printf("\nA vasárnapi napra várt létszám megtelt");
                  }
               }
               else
               {
                  printf("\nInvalid nap van megadva");
               }
            }
            else
            {
               printf("\nValamelyik nap megtelt");
            }

            t = strtok(NULL, delim);
         }
         PrintWorker(workerData, workerCount);
         // Nem szabad hogy számolja a rossz sort hogy felülíródjon kövi olvasáskor
         if (!redo_needed)
         {
            ++workerCount;
         }
         free(inputString);
         if(workerCount == workerDataSize) 
         {
            printf("MOST REALLOC");
            workerDataSize = workerDataSize + reallocExtra;
            workerData = ReallocWorkerData(workerData, workerDataSize, workerCount);
         }
         break;

      case 2:
         SaveWorkerData(workerData, workerCount);
         break;

      case 3:
         WorkerDataFree(workerData, workerDataSize);
         workerData = LoadWorkerData(&workerCount);
         break;
      
      case 4:
         printf("\nAdja meg a módosítani kívánt adathoz tartozó munkás sorszámát\n");
         char* inputToModify = ReadConsoleInput();
         int idToModify;
         bool allgood = false;
         if(atoi(inputToModify) && workerCount >=1) {idToModify = atoi(inputToModify); allgood = true;}
         else printf("Helytelen id");
         
         if(allgood) ModifyRecord(idToModify - 1, workerData, &workerCount);
         break;

      case 5:
      
         printf("\nAdja meg a törölni kívánt adathoz tartozó munkás sorszámát\n");
         char* inputToDelete = ReadConsoleInput();
         int idToDelete;
         allgood = false;
         if(atoi(inputToDelete) && workerCount >=1) {idToDelete = atoi(inputToDelete); allgood = true;}
         else printf("Helytelen id");
         
         if(allgood) DeleteRecord(idToDelete - 1, workerData, &workerCount);
         
         //DeleteRecord(struct worker *wd, int* wdSize)
         break;

      case 6:
         for (int k = 0; k < workerCount; ++k)
         {
            printf("\n%d. munkás: ", k+1);
            PrintWorker(workerData, k);
         }
         printf("\nNapi munkára vártak száma (hétfő-vasárnap):\n");
         PrintWorkerHeadcount();
         printf("Napi munkások száma (hétfő-vasárnap):\n");
         PrintWorkerLoggedHeadcount();
         printf("\nMunkások száma: %d",workerCount);
         break;

      case 7:
         // Should stay empty, cause in this case the program terminates
         
         break;

      default:
         printf("Helytelen opció!\nKérem válasszon a felsoroltakból (0-4) és utána üssön entert!\n");
         opt = '\0';
         break;
      }
      // printf("\nopt: %c\n",opt);

   } while (choice != 7);

   WorkerDataFree(workerData, workerDataSize);
   

   return 0;
}

int GetMenuChoice(int from, int to)
{
   bool inputReadingDone = false;
   char* actInput;
   int choice;
   do
   {
      actInput = ReadConsoleInput();
      if(atoi(actInput)) 
      {
         choice = atoi(actInput);
         if(from <= choice && choice <= to) inputReadingDone = true;
         else printf("%d és %d közötti számot válasszon!\n",to, from);
      }else printf("Egy számot adjon meg!\n");
   }while(!inputReadingDone);

   return choice;
}

char *ReadConsoleInput(void)
{
   char *inputString;
   inputString = (char *)malloc(80 * sizeof(char));
   char c;
   int i = 0;
   while ((c = getchar()) != '\n')
   {

      inputString[i] = c;
      ++i;
   }
   inputString[i] = '\0';
   return inputString;
}

void PrintWorkerHeadcount(void)
{
   printf("\n");
   for (int i = 0; i < 7; ++i)
   {
      printf(" %d", workerWantedHeadcount[i]);
   }
   printf("\n");
}

void PrintWorkerLoggedHeadcount(void)
{
   printf("\n");
   for (int i = 0; i < 7; ++i)
   {
      printf(" %d", workerActualHeadcount[i]);
   }
   printf("\n");
}

void SaveWorkerData(struct worker *wd, int wdSize)
{
   int g;
   char w;
   g = open("savedata.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
   if (g < 0)
   {
      perror("Error at opening the file\n");
      exit(1);
   }

   // First row wanted headcount
   for (int i = 0; i < 7; ++i)
   {
      char convert[5];
      int c = 0;
      sprintf(convert, "%d", workerWantedHeadcount[i]);
      while ((w = convert[c]) != '\0')
      {
         if (write(g, &w, sizeof(w)) != sizeof(w))
         {
            perror("There is a mistake in writing\n");
            exit(1);
         }
         ++c;
      }

      if (i < 6)
      {
         w = ' ';
         if (write(g, &w, sizeof(w)) != sizeof(w))
         {
            perror("There is a mistake in writing\n");
            exit(1);
         }
      }
      else
      {
         w = '\n';
         if (write(g, &w, sizeof(w)) != sizeof(w))
         {
            perror("There is a mistake in writing\n");
            exit(1);
         }
      }
   }

   // Second row actual headcount
   for (int j = 0; j < 7; ++j)
   {
      char convert[5];
      int c = 0;
      sprintf(convert, "%d", workerActualHeadcount[j]);
      while ((w = convert[c]) != '\0')
      {
         if (write(g, &w, sizeof(w)) != sizeof(w))
         {
            perror("There is a mistake in writing\n");
            exit(1);
         }
         ++c;
      }

      if (j < 6)
      {
         w = ' ';
         if (write(g, &w, sizeof(w)) != sizeof(w))
         {
            perror("There is a mistake in writing\n");
            exit(1);
         }
      }
      else
      {
         w = '\n';
         if (write(g, &w, sizeof(w)) != sizeof(w))
         {
            perror("There is a mistake in writing\n");
            exit(1);
         }
      }
   }

   // Workers
   for (int k = 0; k < wdSize; ++k)
   {
      // name
      int l = 0;
      while ((w = wd[k].name[l]) != '\0')
      {
         if (write(g, &w, sizeof(w)) != sizeof(w))
         {
            perror("There is a mistake in writing\n");
            exit(1);
         }
         ++l;
      }
      w = '\n';
      if (write(g, &w, sizeof(w)) != sizeof(w))
      {
         perror("There is a mistake in writing\n");
         exit(1);
      }

      // id
      char convert[5];
      l = 0;
      sprintf(convert, "%d", wd[k].id);
      while ((w = convert[l]) != '\0')
      {
         if (write(g, &w, sizeof(w)) != sizeof(w))
         {
            perror("There is a mistake in writing\n");
            exit(1);
         }
         ++l;
      }
      w = '\n';
      if (write(g, &w, sizeof(w)) != sizeof(w))
      {
         perror("There is a mistake in writing\n");
         exit(1);
      }

      // workdays
      for (int m = 0; m < 7; ++m)
      {
         w = wd[k].workdays[m] + '0';
         if (write(g, &w, sizeof(w)) != sizeof(w))
         {
            perror("There is a mistake in writing\n");
            exit(1);
         }

         if (m < 6)
         {
            w = ' ';
            if (write(g, &w, sizeof(w)) != sizeof(w))
            {
               perror("There is a mistake in writing\n");
               exit(1);
            }
         }
      }
      
      w = '\n';
      if (write(g, &w, sizeof(w)) != sizeof(w))
      {
         perror("There is a mistake in writing\n");
         exit(1);
      }
      
   }

   close(g);
}

struct worker *LoadWorkerData(int *actWorkerCount)
{
   int f;
   f=open("savedata.txt",O_RDONLY);
   if (f<0){ perror("Error at opening the file\n");exit(1);}
   *actWorkerCount = 0;

   struct worker *wData = MallocWorkerData(20);

   char c;
   char row[80];
   bool workerArraysDone = false;
   bool firstArrayDone = false;
   bool secondArrayDone = false;
   int i = 0;
   int j = 0;
   while (read(f,&c,sizeof(c))){
      //Wanted array
      //Actual array
      if(!workerArraysDone)
      {
         if(c != '\n')
         {
            row[i] = c;
            ++i;
         }
         else
         {
            row[i] = '\0';
            printf("\n%s",row);
            const char delim[2] = " ";
            char *t;

            t = strtok(row, delim);
            int k = 0;
            if(!firstArrayDone)
            {
               while (t != NULL)
               {
                  workerWantedHeadcount[k] = atoi(t);
                  printf("\n %d, wanted %d",atoi(t), workerWantedHeadcount[k]);

                  t = strtok(NULL, delim);
                  ++k;
               }
               i = 0;
               firstArrayDone = true;
            }
            else
            {
               while (t != NULL)
               {
                  workerActualHeadcount[k] = atoi(t);
                  printf("\n %d, wanted %d",atoi(t), workerActualHeadcount[k]);

                  t = strtok(NULL, delim);
                  ++k;
               }
               i = 0;
               workerArraysDone = true;
            }
         }
      }
      else  //Workers
      {
         if(c != '\n')
         {
            row[i] = c;
            ++i;
         }
         else
         {
            row[i] = '\0';
            printf("\n%s",row);
            int actRecord = *actWorkerCount;
            if(j%3 == 0)      //Name
            {
               printf("\nNEVET OLVASOK");
               strcpy(wData[actRecord].name, row);
            }
            else if(j%3 == 1) //ID
            {
               wData[actRecord].id = atoi(row);
            }
            else if(j%3 == 2) //Workdays
            {
               const char delim[2] = " ";
               char *t;

               t = strtok(row, delim);
               int k = 0;
               while (t != NULL)
               {
                  wData[actRecord].workdays[k] = atoi(t);

                  t = strtok(NULL, delim);
                  ++k;
               }
               *actWorkerCount = ++actRecord;
            }
            i=0;
            ++j;
         }
      }
   }
   close(f);
   return wData;
}

void WorkerDataFree(struct worker *wData, int size)
{
   for (int i = 0; i < size; ++i)
   {
      free(wData[i].name);
      free(wData[i].workdays);
   }

   free(wData);
}

struct worker *MallocWorkerData(int initialSize)
{
   struct worker *wd;
   wd = (struct worker *)malloc(initialSize * sizeof(struct worker));
   for (int i = 0; i < initialSize; ++i)
   {
      wd[i].name = (char *)malloc(100 * sizeof(char));
      wd[i].workdays = (bool *)malloc(7 * sizeof(bool));
   }

   return wd;
}

struct worker *ReallocWorkerData(struct worker *previousWorkerData, int newSize, int oldSize)
{
   previousWorkerData = realloc(previousWorkerData, (newSize * sizeof(struct worker)));

   for(int i = oldSize; i < newSize; ++i)
   {
      previousWorkerData[i].name = (char *)malloc(100 * sizeof(char));
      previousWorkerData[i].workdays = (bool *)malloc(7 * sizeof(bool));
   }

   return previousWorkerData;
}

void PrintWorker(struct worker *wd, int id)
{
   printf("\nworker name: %s", wd[id].name);
   printf("\nworker id: %d", wd[id].id);
   printf("\nworkdays: ");
   for (int i = 0; i < 7; ++i)
   {
      printf("%d ", wd[id].workdays[i]);
   }
}

void ReadInWorkerHeadcount(void)
{
   printf("\nKérem adja meg a várt munkások számát a hét napjaira");
   char *in;
   for (int i = 0; i < 7; ++i)
   {
      if (i == 0)
      {
         do
         {
            printf("\nhétfő: ");
            in = ReadConsoleInput();
            if (atoi(in))
            {
               workerWantedHeadcount[i] = atoi(in);
            }
         } while (!atoi(in));
      }
      else if (i == 1)
      {
         do
         {
            printf("\nkedd: ");
            in = ReadConsoleInput();
            if (atoi(in))
            {
               workerWantedHeadcount[i] = atoi(in);
            }
         } while (!atoi(in));
      }
      else if (i == 2)
      {
         do
         {
            printf("\nszerda: ");
            in = ReadConsoleInput();
            if (atoi(in))
            {
               workerWantedHeadcount[i] = atoi(in);
            }
         } while (!atoi(in));
      }
      else if (i == 3)
      {
         do
         {
            printf("\ncsütörtök: ");
            in = ReadConsoleInput();
            if (atoi(in))
            {
               workerWantedHeadcount[i] = atoi(in);
            }
         } while (!atoi(in));
      }
      else if (i == 4)
      {
         do
         {
            printf("\npéntek: ");
            in = ReadConsoleInput();
            if (atoi(in))
            {
               workerWantedHeadcount[i] = atoi(in);
            }
         } while (!atoi(in));
      }
      else if (i == 5)
      {
         do
         {
            printf("\nszombat: ");
            in = ReadConsoleInput();
            if (atoi(in))
            {
               workerWantedHeadcount[i] = atoi(in);
            }
         } while (!atoi(in));
      }
      else if (i == 6)
      {
         do
         {
            printf("\nvasárnap: ");
            in = ReadConsoleInput();
            if (atoi(in))
            {
               workerWantedHeadcount[i] = atoi(in);
            }
         } while (!atoi(in));
      }
   }
}

void DeleteRecord(int idToDelete, struct worker *wd, int* wdSize)
{
   
   int recordCount = (*wdSize);
   if(idToDelete > recordCount || idToDelete < 0) 
   {
      printf("Helytelen törölni kívánt id");
      return;
   }
   
   for(int i = idToDelete; i < (recordCount - 1); ++i)
   {
      strcpy(wd[i].name, wd[i+1].name);
      for(int j = 0; j < 7; ++j) wd[i].workdays[j] = wd[i+1].workdays[j];
   }
   *wdSize = --recordCount;
}

void ModifyRecord(int idToModify, struct worker *wd, int *wdSize)
{
   int recordCount = (*wdSize);
   if (idToModify > recordCount || idToModify < 0)
   {
      printf("Helytelen módosítani kívánt id");
      return;
   }

   PrintWorker(wd, idToModify);

   bool redo_needed = false;

   do
   {
      printf("\nKérem adja meg a nevét, illetve a napokat amikor munkára jelentkezne!\n");

      char *inputString = ReadConsoleInput();

      printf("\n%s\n", inputString);
      const char delim[2] = " ";
      char *t;

      t = strtok(inputString, delim);
      bool first_value = true;

      while (t != NULL)
      {
         printf("\n%s", t);

         if (first_value)
         {
            strcpy(wd[idToModify].name, t);
            first_value = false;
         }
         else if (!redo_needed)
         {

            if (strcmp(t, "hétfő") == 0)
            {
               if (workerActualHeadcount[0] < workerWantedHeadcount[0])
               {
                  wd[idToModify].workdays[0] = true;
                  ++workerActualHeadcount[0];
               }
               else
               {
                  redo_needed = true;
                  printf("\nA hétfői napra várt létszám megtelt");
               }
            }
            else if (strcmp(t, "kedd") == 0)
            {
               if (workerActualHeadcount[1] < workerWantedHeadcount[1])
               {
                  wd[idToModify].workdays[1] = true;
                  ++workerActualHeadcount[1];
               }
               else
               {
                  redo_needed = true;
                  printf("\nA keddi napra várt létszám megtelt");
               }
            }
            else if (strcmp(t, "szerda") == 0)
            {
               if (workerActualHeadcount[2] < workerWantedHeadcount[2])
               {
                  wd[idToModify].workdays[2] = true;
                  ++workerActualHeadcount[2];
               }
               else
               {
                  redo_needed = true;
                  printf("\nA szerdai napra várt létszám megtelt");
               }
            }
            else if (strcmp(t, "csütörtök") == 0)
            {
               if (workerActualHeadcount[3] < workerWantedHeadcount[3])
               {
                  wd[idToModify].workdays[3] = true;
                  ++workerActualHeadcount[3];
               }
               else
               {
                  redo_needed = true;
                  printf("\nA csütörtöki napra várt létszám megtelt");
               }
            }
            else if (strcmp(t, "péntek") == 0)
            {
               if (workerActualHeadcount[4] < workerWantedHeadcount[4])
               {
                  wd[idToModify].workdays[4] = true;
                  ++workerActualHeadcount[4];
               }
               else
               {
                  redo_needed = true;
                  printf("\nA pénteki napra várt létszám megtelt");
               }
            }
            else if (strcmp(t, "szombat") == 0)
            {
               if (workerActualHeadcount[5] < workerWantedHeadcount[5])
               {
                  wd[idToModify].workdays[5] = true;
                  ++workerActualHeadcount[5];
               }
               else
               {
                  redo_needed = true;
                  printf("\nA szombati napra várt létszám megtelt");
               }
            }
            else if (strcmp(t, "vasárnap") == 0)
            {
               if (workerActualHeadcount[6] < workerWantedHeadcount[6])
               {
                  wd[idToModify].workdays[6] = true;
                  ++workerActualHeadcount[6];
               }
               else
               {
                  redo_needed = true;
                  printf("\nA vasárnapi napra várt létszám megtelt");
               }
            }
            else
            {
               printf("\nInvalid nap van megadva");
            }
         }
         else
         {
            printf("\nValamelyik nap megtelt");
         }

         t = strtok(NULL, delim);
      }
      PrintWorker(wd, idToModify);
      free(inputString);
   } while (redo_needed);
}

