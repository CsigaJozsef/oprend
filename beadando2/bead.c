#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>     //open,creat
#include <sys/types.h> //open
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <wait.h>

/*-------------------------------------------TODO---------------------------------------------*/
/**TODO:
 *
 * /

/*-------------------------------------------Structs---------------------------------------------*/
struct worker
{
   int id;
   char *name;
   bool *workdays;
};

struct uzenet
{
   long mtype; // ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
   char mtext[1024];
};

void handler(int signumber)
{
   printf("[handler] Signal with number %i has arrived\n", signumber);
}

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
void DeleteRecord(int idToDelete, struct worker *wd, int *wdSize);
void ModifyRecord(int idToModify, struct worker *wd, int *wdSize);
char* GetNamesForSpecificDay(int dayNumber, struct worker *wd, int wdCount);
int kuld(int uzenetsor, int uzenetkod);
int fogad(int uzenetsor, int uzenetkod);
/*-----------------------------------------------------------------------------------------------*/
int main(int argc, char **argv)
{

   signal(SIGUSR1, handler);
   signal(SIGUSR2, handler);

   char opt = '\0';
   int workerDataSize = 2;
   int workerCount = 0;

   struct worker *workerData = MallocWorkerData(workerDataSize);

   bool preparationDone = false;

   printf("\nKérem válasszon az alábbi menüpontokból!\n[1] Várt munkások számának megadása\n[2] Munkások betöltése file-ból\n");

   int choice = GetMenuChoice(1, 2);

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
      printf("\nKérem válassza ki a végrehajtandó műveletet az alábbiak közül!\n[1] Új munkás felvétele\n[2] Adatok mentése\n[3] Adatok betöltése\n[4] Adat módosítása\n[5] Adat törlése\n[6] Adatok kiírása\n[7] Reggeli busz indítása\n[8] Kilépés (A nem mentett adatok elvesznek)\n");
      choice = GetMenuChoice(1, 8);
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
         if (workerCount == workerDataSize)
         {
            // printf("MOST REALLOC");
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
         char *inputToModify = ReadConsoleInput();
         int idToModify;
         bool allgood = false;
         if (atoi(inputToModify) && workerCount >= 1)
         {
            idToModify = atoi(inputToModify);
            allgood = true;
         }
         else
            printf("Helytelen id");

         if (allgood)
            ModifyRecord(idToModify - 1, workerData, &workerCount);
         break;

      case 5:

         printf("\nAdja meg a törölni kívánt adathoz tartozó munkás sorszámát\n");
         char *inputToDelete = ReadConsoleInput();
         int idToDelete;
         allgood = false;
         if (atoi(inputToDelete) && workerCount >= 1)
         {
            idToDelete = atoi(inputToDelete);
            allgood = true;
         }
         else
            printf("Helytelen id");

         if (allgood)
            DeleteRecord(idToDelete - 1, workerData, &workerCount);

         // DeleteRecord(struct worker *wd, int* wdSize)
         break;

      case 6:
         for (int k = 0; k < workerCount; ++k)
         {
            printf("\n%d. munkás: ", k + 1);
            PrintWorker(workerData, k);
         }
         printf("\nNapi munkára vártak száma (hétfő-vasárnap):\n");
         PrintWorkerHeadcount();
         printf("Napi munkások száma (hétfő-vasárnap):\n");
         PrintWorkerLoggedHeadcount();
         printf("\nMunkások száma: %d", workerCount);
         break;

      case 7: ;

         bool twoBusesNeeded;
         twoBusesNeeded = (workerCount > 5);

         // pipe 1
         int pipefd1[2];     // unnamed pipe file descriptor array
         char message1[100]; // char array for reading from pipe

         if (pipe(pipefd1) == -1)
         {
            perror("Hiba a pipe 1 nyitaskor!");
            exit(EXIT_FAILURE);
         }

         // pipe 2
         int pipefd2[2];     // unnamed pipe file descriptor array
         char message2[100]; // char array for reading from pipe

         if (pipe(pipefd2) == -1)
         {
            perror("Hiba a pipe 2 nyitaskor!");
            exit(EXIT_FAILURE);
         }

         int uzenetsor;
         key_t kulcs;

         // msgget needs a key, amelyet az ftok gener�l
         //.
         kulcs = ftok(argv[0], 1);
         printf("A kulcs: %d\n", kulcs);
         uzenetsor = msgget(kulcs, 0600 | IPC_CREAT);
         if (uzenetsor < 0)
         {
            perror("msgget");
            return 1;
         }

         pid_t child = fork(); // forks make a copy of variables
         if (child < 0)
         {
            perror("The fork calling was not succesful\n");
            exit(1);
         }
         if (child > 0) // the parent process, it can see the returning value of fork - the child variable!
         {

            if (twoBusesNeeded)
            {

               pause(); // waits till a signal arrive
               printf("First signal arrived: %i\n", SIGUSR1);

               // Child 2 gets the people
               close(pipefd1[0]); // Usually we close unused read end
               char* wanted1 = GetNamesForSpecificDay(4, workerData, workerCount);
               write(pipefd1[1], wanted1, sizeof(wanted1));
               close(pipefd1[1]); // Closing write descriptor
               printf("Szulo beirta az adatokat a csobe 1!\n");
               fflush(NULL); // flushes all write buffers (not necessary)

               fogad(uzenetsor, 3);
               printf("\n");

               pause(); ////waits till second signal arrive
               printf("Second signal arrived: %i\n", SIGUSR2);

               // Child 1 gets the people
               printf("First signal arrived: %i\n", SIGUSR1);
               close(pipefd2[0]); // Usually we close unused read end
               char* wanted2 = GetNamesForSpecificDay(0, workerData, workerCount);
               write(pipefd2[1], wanted2, sizeof(wanted2));
               close(pipefd2[1]); // Closing write descriptor
               printf("Szulo beirta az adatokat a csobe 2!\n");
               fflush(NULL); // flushes all write buffers (not necessary)

               //sleep(1);

               fogad(uzenetsor, 6);

               int status = msgctl(uzenetsor, IPC_RMID, NULL);
               if (status < 0)
               {
                  perror("msgctl");
               }
               free(wanted2);
               free(wanted1);
            }
            else
            {
               pause(); // waits till a signal arrive
               printf("First signal arrived: %i\n", SIGUSR1);

               close(pipefd1[0]); // Usually we close unused read end
               char* wanted = GetNamesForSpecificDay(2, workerData, workerCount);
               write(pipefd1[1], wanted, sizeof(wanted));
               close(pipefd1[1]); // Closing write descriptor
               printf("Szulo beirta az adatokat a csobe!\n");
               fflush(NULL); // flushes all write buffers (not necessary)

               fogad(uzenetsor, 3);

               int status = msgctl(uzenetsor, IPC_RMID, NULL);
               if (status < 0)
               {
                  perror("msgctl");
               }

               free(wanted);
            }

            int status;
            wait(&status);
            printf("\nParent process ended\n");
         }
         else // child process
         {
            pid_t szoleszet = getppid();
            if (!twoBusesNeeded)
            {
               printf("Waits 3 seconds, then send a SIGUSR1 %i signal\n", SIGUSR1);
               sleep(1);
               kill(szoleszet, SIGUSR1);

               close(pipefd1[1]); // Usually we close the unused write end
               printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
               read(pipefd1[0], message1, sizeof(message1)); // reading max 100 chars
               printf("Gyerek olvasta uzenet: %s", message1);
               printf("\n");
               close(pipefd1[0]); // finally we close the used read end

               kuld(uzenetsor, 3); // child 1 sends a message.

               printf("\nChild process ended\n");
               // exit(0);
            }
            else
            {
               pid_t child2 = fork();
               if (child2 < 0)
               {
                  perror("The fork calling was not succesful\n");
                  exit(1);
               }
               if (child2 > 0) // child 1 (a parent also)
               {
                  sleep(1);
                  kill(szoleszet, SIGUSR1);

                  close(pipefd1[1]); // Usually we close the unused write end
                  printf("Gyerek 1 elkezdi olvasni a csobol az adatokat!\n");
                  read(pipefd1[0], message1, sizeof(message1)); // reading max 100 chars
                  printf("Gyerek 1 olvasta uzenet: %s\n", message1);
                  close(pipefd1[0]); // finally we close the used read end

                  kuld(uzenetsor, 3); // child 1 sends a message.

                  int status2;
                  wait(&status2);
                  printf("\nChild process 1 ended\n");
               }
               else // child 2
               {
                  sleep(3);
                  kill(szoleszet, SIGUSR2);

                  sleep(1);
                  close(pipefd2[1]); // Usually we close the unused write end
                  printf("Gyerek 2 elkezdi olvasni a csobol az adatokat!\n");
                  read(pipefd2[0], message2, sizeof(message2)); // reading max 100 chars
                  printf("Gyerek 2 olvasta uzenet: %s\n", message2);
                  close(pipefd2[0]); // finally we close the used read end

                  printf("Gyerek 2 elküldte a behozottakat");
                  kuld(uzenetsor, 6); // child 2 sends a message.

                  printf("\nChild process 2 ended\n");
                  exit(0);
               }
            }
            // sleep(1);
            printf("BIG child process ended");
            exit(0);
         }

         break;

      case 8:
         // Should stay empty, cause in this case the program terminates

         break;

      default:
         printf("Helytelen opció!\nKérem válasszon a felsoroltakból (0-4) és utána üssön entert!\n");
         opt = '\0';
         break;
      }
      // printf("\nopt: %c\n",opt);

   } while (choice != 8);

   WorkerDataFree(workerData, workerDataSize);

   return 0;
}

// sendig a message
int kuld(int uzenetsor, int uzenetkod)
{
   const struct uzenet uz = {uzenetkod, "Busz megérkezett minden utassal"};
   int status;

   status = msgsnd(uzenetsor, &uz, strlen(uz.mtext) + 1, 0);
   // a 3. param ilyen is lehet: sizeof(uz.mtext)
   // a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
   if (status < 0)
      perror("msgsnd");
   return 0;
}

// receiving a message.
int fogad(int uzenetsor, int uzenetkod)
{
   struct uzenet uz;
   int status;
   // az utolso parameter(0) az uzenet azonositoszama
   // ha az 0, akkor a sor elso uzenetet vesszuk ki
   // ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
   // vesszuk ki a sorbol
   status = msgrcv(uzenetsor, &uz, 1024, uzenetkod, 0);

   if (status < 0)
      perror("msgsnd");
   else
      printf("A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext);
   return 0;
}

char* GetNamesForSpecificDay(int dayNumber, struct worker *wd, int wdCount){
   char *inputString;
   inputString = (char *)malloc(80 * sizeof(char));
   strcpy(inputString, "");

   int stringIndex;
   bool first = true;

   for(int i = 0; i < wdCount; ++i){
      if(wd[i].workdays[dayNumber] && first){
         strcat(inputString, wd[i].name);
         first = false;
      }
      else if(wd[i].workdays[dayNumber] && !first){
         strcat(inputString, " ");
         strcat(inputString, wd[i].name);
      }
   }

   return inputString;
}

int GetMenuChoice(int from, int to)
{
   bool inputReadingDone = false;
   char *actInput;
   int choice;
   do
   {
      actInput = ReadConsoleInput();
      if (atoi(actInput))
      {
         choice = atoi(actInput);
         if (from <= choice && choice <= to)
            inputReadingDone = true;
         else
            printf("%d és %d közötti számot válasszon!\n", to, from);
      }
      else
         printf("Egy számot adjon meg!\n");
   } while (!inputReadingDone);

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
   f = open("savedata.txt", O_RDONLY);
   if (f < 0)
   {
      perror("Error at opening the file\n");
      exit(1);
   }
   *actWorkerCount = 0;

   struct worker *wData = MallocWorkerData(20);

   char c;
   char row[80];
   bool workerArraysDone = false;
   bool firstArrayDone = false;
   bool secondArrayDone = false;
   int i = 0;
   int j = 0;
   while (read(f, &c, sizeof(c)))
   {
      // Wanted array
      // Actual array
      if (!workerArraysDone)
      {
         if (c != '\n')
         {
            row[i] = c;
            ++i;
         }
         else
         {
            row[i] = '\0';
            printf("\n%s", row);
            const char delim[2] = " ";
            char *t;

            t = strtok(row, delim);
            int k = 0;
            if (!firstArrayDone)
            {
               while (t != NULL)
               {
                  workerWantedHeadcount[k] = atoi(t);
                  printf("\n %d, wanted %d", atoi(t), workerWantedHeadcount[k]);

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
                  printf("\n %d, wanted %d", atoi(t), workerActualHeadcount[k]);

                  t = strtok(NULL, delim);
                  ++k;
               }
               i = 0;
               workerArraysDone = true;
            }
         }
      }
      else // Workers
      {
         if (c != '\n')
         {
            row[i] = c;
            ++i;
         }
         else
         {
            row[i] = '\0';
            printf("\n%s", row);
            int actRecord = *actWorkerCount;
            if (j % 3 == 0) // Name
            {
               printf("\nNEVET OLVASOK");
               strcpy(wData[actRecord].name, row);
            }
            else if (j % 3 == 1) // ID
            {
               wData[actRecord].id = atoi(row);
            }
            else if (j % 3 == 2) // Workdays
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
            i = 0;
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

   for (int i = oldSize; i < newSize; ++i)
   {
      previousWorkerData[i].name = (char *)malloc(100 * sizeof(char));
      // 0-zni mindent is
      strcpy(previousWorkerData[i].name, "\0");
      previousWorkerData[i].workdays = (bool *)malloc(7 * sizeof(bool));
      // 0-zni mindent is
      for (int j = 0; j < 7; ++j)
         previousWorkerData[i].workdays[j] = false;
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

void DeleteRecord(int idToDelete, struct worker *wd, int *wdSize)
{

   int recordCount = (*wdSize);
   if (idToDelete > recordCount || idToDelete < 0)
   {
      printf("Helytelen törölni kívánt id");
      return;
   }

   for (int i = idToDelete; i < (recordCount - 1); ++i)
   {
      strcpy(wd[i].name, wd[i + 1].name);
      for (int j = 0; j < 7; ++j)
         wd[i].workdays[j] = wd[i + 1].workdays[j];
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

   // Kinullázni a napokat!!
   for (int i = 0; i < 7; ++i)
      wd[idToModify].workdays[i] = false;

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
