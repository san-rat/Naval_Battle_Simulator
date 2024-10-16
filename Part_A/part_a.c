 /* This program is a Advanced Naval Battle simulator program. 
   
   You will have to give the dementions of the battlefield, the battleship you choose to fight with, where to place the battleship within the battlefield, and the number of enemy escort ships within the battlefield.
   
   The velocities of the battleship as well as the escortships will be randomly generated within the limits and the escort ships will also be randomly generated.
   
   In this program you will see where the ships are placed within the battlefield, and you will find the destroyed ships on the battlefield as well.
   Destroyed battleship will be seen as "X", and the destroyed escort ships will be seen as "x".
   
   All the ships will be stationary targets and they will not move in any part of the program.

   All the ships will be destroyed with one hit from any of the enemy ships.(Battleship or Escort ship)

   A file (Battle_log.txt) will be created in the same directory as the program and the file will have all the information about the battle after the simulation has been run once.
   */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define pi 3.141592653589793238462643383279502884197169399375105820
#define best_angle 45
#define gravity 9.81

//Structure of a Battleship.
typedef struct {           
  char ship_name[20]; 
  char notation[2];  
  char gun[30]; 
  int max_velocity;  
  int x;  
  int y;  
} battle_ship;

//Structure of an Escortship.
typedef struct {          
  char notation[4]; 
  char ship_name[30]; 
  char gun[30];  
  float impact_power; 
  int angle_range; 
  int min_angle; 
  int min_velocity; 
  float max_velocity; 
  int x; 
  int y; 
  int index; 
  int status; 
  int max_angle; 
} escort_ship;

//Function to generate a random number.
int random_number(int min, int max){    
  
  return min + rand() % (max - min + 1);
}

//Function to convert degrees to radians.
double radian(int degrees){
  
  return degrees * (pi / 180.0);
}

//Function to find the shooting range of a ship.
double range(int velocity, int angle){
  
  return (pow(velocity, 2) * sin(2 * radian(angle))) / gravity;
}

//Function to find if a ship is within a range of another ship.
int in_range(escort_ship E_ships, battle_ship B_ship){
  
  double distance = sqrt(pow(B_ship.x - E_ships.x, 2) + pow(B_ship.y - E_ships.y, 2));
  return (distance <= range(B_ship.max_velocity, best_angle));
}

//Function to calculate the distance betweent two ships.
float short_distance(escort_ship E_ships, battle_ship B_ship){
  
  return sqrt(pow(B_ship.x - E_ships.x, 2) + pow(B_ship.y - E_ships.y, 2));
}

//Function where the battleships attack the escortships.
void b_attack(escort_ship E_ships[], int e_ships, battle_ship B_ship, int dx, int dy, int *targets){
  
  for(int i = 0; i < e_ships; i++){
    
    if(in_range(E_ships[i], B_ship)){
      (*targets)++;
      E_ships[i].status = 1;
    }
  }
}

//Function where the escortships attack the battleships.
void e_attack(escort_ship E_ships[], int e_ships, battle_ship B_ship, int *index, int *status){

  float upper_limit=0, lower_limit=0, e_to_b_distance=0;

  for(int i = 0; i < e_ships; i++){
    
    if(E_ships[i].max_angle >= best_angle){
      upper_limit = range(E_ships[i].max_velocity, best_angle);
    }
    else{
      upper_limit = range(E_ships[i].max_velocity, E_ships[i].max_angle);
    }
    
    lower_limit = range(E_ships[i].min_velocity, E_ships[i].min_angle);
    e_to_b_distance = short_distance(E_ships[i], B_ship);

    if(e_to_b_distance <= upper_limit && e_to_b_distance >= lower_limit){
      *status = 1;
      *index = E_ships[i].index; //If the battleship is within the range of the escortship, the battleship is destroyed(*status = 1), and the the index of the escortship is returned.
    }
  }
}

//Function to calculate the maximum velocity of all the escortships.
void calculateMaxV(escort_ship E_ships[], battle_ship B_ship){
  for(int i = 0; i < 5; i++){
    
    if(i == 0) {
      E_ships[i].max_velocity = 1.2 * B_ship.max_velocity;  //Maximum velocity of escortship E_A.
    } 
    else {
      E_ships[i].max_velocity = random_number(1,B_ship.max_velocity-1);  //Make the Maximum velocity of the escortships lower that the battleships maximum velocity.
    }
  }
}

//Function to print the battlefield.(canvas)
void battlefield(escort_ship E_ships[], battle_ship B_ship, int dx, int dy, int e_ships, int **canvas, int B_status){
  
  for(int i = 0; i < dy; i++){
    
    for(int j = 0; j < dx; j++){
        canvas[i][j] = 0; 
    }
  }

  if(B_status == 1){
    canvas[dy - B_ship.y][B_ship.x - 1] = -1; 
  }
  else{
    canvas[dy - B_ship.y][B_ship.x - 1] = -2;
  }

  for(int i = 0; i < e_ships; i++){
    
    if(E_ships[i].status == 1){
      canvas[dy - E_ships[i].y][E_ships[i].x - 1] = -3;
    }
    else{
      canvas[dy - E_ships[i].y][E_ships[i].x - 1] = E_ships[i].index;
    }
  }
  
  printf("\n"); //printing the battlefield.
  
  for(int i=0; i<dy; i++){
    
    for(int j=0; j<dx; j++){
      
      if(canvas[i][j] == 0){
        printf(". ");
      }
      else if(canvas[i][j] == -1){
        printf("X ");
      }
      else if(canvas[i][j] == -2){
        printf("B ");
      }
      else if(canvas[i][j] == -3){
        printf("x ");
      }
      else if(canvas[i][j]>=10){
        printf("%d", canvas[i][j]);
      }
      else{
        printf("%d ",canvas[i][j]);
      }
    }
    printf("\n");
  }
}

//Function that simulate the full battle.
void battle(battle_ship battle_ships[], escort_ship escort_ships[]){
  
  int dx, dy;
  printf("\nEnter the dimenions(x y) of the battlefield: "); //Input the dimentions of the battlefield.
  scanf("%d %d", &dx, &dy);

  while(!(dx == dy && dx > 0 && dy > 0)){
    
    printf("\nThe battlefield must be a square.\nPlease enter the dimensions(x, y) again:");
    scanf("%d %d", &dx, &dy);
  }

  printf("\nBattleship Information:\n");
  printf("\n%-23s %-10s %-25s %-16s\n", "Battleship Name", "Notation", "Gun Name", "Maximum Velocity");
  
  for (int i = 0; i < 4; i++) {
    
    printf("%-26s %-7s %-33s %-16d\n", battle_ships[i].ship_name, battle_ships[i].notation, battle_ships[i].gun, battle_ships[i].max_velocity);
  }

  battle_ship B_ship;

  char choice;
  printf("\nChoose your Battleship type: "); //Input the type of battleship.
  scanf(" %c", &choice);

  while(!(choice == 'U' || choice == 'u' || choice == 'M' || choice == 'm' || choice == 'R' || choice == 'r' || choice == 'S' || choice == 's')){
    
    printf("\nInvaild choice.\nPlease choose a valid Battleship type: ");
    scanf(" %c", &choice);
  }

  if (choice == 'u' || choice == 'U') {
    
    printf("\n\nCongradulations!");
    printf("\nYou selected %s Battleship\n", battle_ships[0].ship_name);
    
    B_ship = battle_ships[0];  //Copying the data of the selected battleship to the B_ship array.(variable)
    
    calculateMaxV(escort_ships, B_ship);  //Calling the function to calculate the Maximum Velocity of all the Escort Ships.
  }
  
  if (choice == 'm' || choice == 'M') {
    
    printf("Congradulations!");
    printf("\nYou selected %s Battleship\n", battle_ships[1].ship_name);
    
    B_ship = battle_ships[1];
    
    calculateMaxV(escort_ships, B_ship);
    
  }
  
  if (choice == 'r' || choice == 'R') {
    
    printf("Congradulations!");
    printf("\nYou selected %s Battleship\n", battle_ships[2].ship_name);
    
    B_ship = battle_ships[2];
    
    calculateMaxV(escort_ships, B_ship);
  }
  
  if (choice == 's' || choice == 'S') {
    
    printf("Congradulations!");
    printf("\nYou selected %s Battleship\n", battle_ships[3].ship_name);
    
    B_ship = battle_ships[3];
    
    calculateMaxV(escort_ships, B_ship);
  }

  printf("\nInput coordinates for your Battleship: ");  //Input the coodinates of the battleship.
  scanf("%d %d", &B_ship.x, &B_ship.y);

  while(!(B_ship.x <= dx && B_ship.x >= 1 && B_ship.y <= dy && B_ship.y >= 1)){
    
    printf("\nThe Battleship is not inside the battlefield.\nPlease enter valid coordinates for you Battleship: ");
    scanf( "%d %d", &B_ship.x, &B_ship.y);
  }

  int e_ships;
  printf("\nEnter number of Escort ships: ");  //Input the number of Escort ships.
  scanf("%d",&e_ships);

  while(!(e_ships >= 1 && e_ships < dx * dy)){
    
    printf("\nThe number of Escort ships must be between 1 and %d.\nPlease enter a valid number: ", dx * dy - 1);
    scanf(" %d", &e_ships);
  }

  escort_ship E_ships[e_ships];   //Declaring an array of structures to copy the data of the ramdomly generated escort ships.

  for(int j = 0; j < e_ships; j++){
    
    int index_no = random_number(0,4);    //Generating a number between 0 and 4.
    E_ships[j] = escort_ships[index_no];  //Copying attributes to the arrays of structures.
    
    E_ships[j].x = random_number(1,dx-1);  //Randomly generating the coordinates in the battlefield.
    E_ships[j].y = random_number(1,dy-1);
    
    E_ships[j].index = j+1;  //Assigning an index number to all the escort ships.
  }

  int **canvas = (int **)malloc(dy * sizeof(int *));
  
  for(int i = 0; i < dy; i++){
    
    canvas[i] = (int *)malloc(dx * sizeof(int));
  }
  
  for(int i = 0; i < e_ships; i++){
    
    E_ships[i].max_angle = E_ships[i].min_angle + E_ships[i].angle_range; //Calculating the maximum angle of the escort ships.
  }

  int Eshipsinrange = 0;
  
  int This_E_hit_B;
  
  int Bstatus=0; //Using a variable to check the status of the battleship.
  
printf("\n\nBattlefield before battle.");
  
  battlefield(E_ships, B_ship, dx, dy, e_ships, canvas, Bstatus); 
  
  b_attack(E_ships, e_ships, B_ship, dx, dy, &Eshipsinrange);
    
  e_attack(E_ships, e_ships, B_ship, &This_E_hit_B ,&Bstatus);

  printf("\n\nBattlefield after battlefield.");

  battlefield(E_ships, B_ship, dx, dy, e_ships, canvas, Bstatus);

  
  for (int i = 0; i < dy; i++) { // Free the dynamically allocated memory.
      
    free(canvas[i]);
  }
  
  free(canvas);

  
  char Bcoordinates[10];  //Assigning the coordinates of the Battleship into a 1D array.
  sprintf(Bcoordinates, "(%d, %d)", B_ship.x, B_ship.y);

  char Ecoordinates[e_ships][10];  //Assigning the coordinates of each Escort Ship into a 2D array.
  
  for(int n = 0; n < e_ships; n++){
    
    sprintf(Ecoordinates[n], "(%d, %d)", E_ships[n].x, E_ships[n].y);
  }

  FILE *fileptr;  //Declaring the file pointer to a text file.

  fileptr = fopen("Battle_data.txt", "w");   //Opening a text file to write mode.
  if (fileptr == NULL) {
     printf("\nFile cannot be created\n");
    return;
  }
  
  fprintf(fileptr, "Battlefield Dimensions: %d x %d\n", dx, dy); //Storing the data of the Battlefield into the text file.
  fprintf(fileptr, "\nBattleship:\n");
  fprintf(fileptr, "%-6s %-18s %-17s\n", "Type","Initial Position","Maximum Velocity");
  fprintf(fileptr, "%-9c %-18s %-17d\n", choice, Bcoordinates, B_ship.max_velocity);
  fprintf(fileptr, "\nEscort Ships:\n");
  fprintf(fileptr, "%-6s %-4s %-10s %-7s %-7s %-7s %-7s\n", "Type","ID","Position","Min V","Max V","Min θ","Max θ");
  
  for(int k = 0; k < e_ships; k++){
    
    fprintf(fileptr,"%-7s %-3d %-12s %-5d %-8.2f %-6d %-7d\n", E_ships[k].notation, E_ships[k].index, Ecoordinates[k], E_ships[k].min_velocity, E_ships[k].max_velocity, E_ships[k].min_angle, E_ships[k].max_angle);
  }
  
  fclose(fileptr);  //Closing the file.

}

//Function for the 'Instructions' part of the menu.
void instructions(battle_ship battleships[], escort_ship escortships[]){

  printf("\n\nBattleship Information:\n");  //Printing all the data of the Battleships.
  printf("\n%-23s %-10s %-25s %-16s\n", "Battleship Name", "Notation", "Gun Name", "Maximum Velocity");
  
  for (int i = 0; i < 4; i++) {
    
    printf("%-26s %-7s %-33s %-16d\n", battleships[i].ship_name, battleships[i].notation, battleships[i].gun, battleships[i].max_velocity);
  }

  printf("\n\n\nEscort Ship Information:\n");  //Printing all the data of the escort ships.
  printf("\n%-15s %-25s %-27s %-15s %-13s %-15s %-17s\n", "Type Notation", "Type Name", "Gun Name", "Impact Power", "Angle Range", "Minimum Angle", "Minimum Velocity");
  
  for (int i = 0; i < 5; i++) {
    
    printf("%-15s %-25s %-30s %-18.2f %-13d %-15d %-17d\n", escortships[i].notation, escortships[i].ship_name, escortships[i].gun, escortships[i].impact_power, escortships[i].angle_range, escortships[i].min_angle, escortships[i].min_velocity);
  }

  printf("\n\n\nFirst provide the dimension of the battlefield. (Please note that the battlefield must be a square)\n\nThen, choose a type of Battleship from the catalogue of batttleships given to you and enter the 'Notation' of the chosen Battleship.\n\nAfter that, provide the coordinates of the Battleship within the battlefield.\n\nAnd finally, enter the number of Escort ships you want to have in the simulation.\n\nThe type, position and the maximum velocity of each Escort ships will be randomly generated.\n\nPlease note that the maximum velocity of the E_A Escort ships will be 1.2 times the maximum velocity of the Battleship.\n\nThe maximum velocity of the other randomly generated Escort ships will be lower than the maximum velocity of the Battleship.\n\n");

  
}

//Main Function
int main(){
  
  srand(time(NULL)); //Seeding the random number generator.

  battle_ship battleships[4] = {
  {"USS Iowa (BB-61)", "U", "50-caliber Mark VII gun", random_number(0, 10)},   //Giving the data to the battle_ship structure as an array.
  {"MS King George V", "M", "(356 mm) Mark VII gun", random_number(0, 10)},
  {"Richelieu", "R", "(15 inch) Mle 1935 gun", random_number(0, 10)},
  {"Sovetsky Soyuz-class", "S", "(16 inch) B-37 gun", random_number(0, 10)}};

  escort_ship escortships[5] = {    //Giving the data to escort_ship structure as an array.
  {"E_A", "1936 A-Class Destroyer", "SK C/34 naval gun", 0.08, 20, random_number(5, 15), random_number(1, 3)},
  {"E_B", "Gabbiano-Class Corvette", "L/47 dual purpose gun", 0.06, 30, random_number(5, 15), random_number(1, 3)},
  {"E_C", "Matsu-Class Destroyer", "Type 89 dual purpose gun", 0.07, 25, random_number(5, 10), random_number(1, 3)},
  {"E_D", "F-Class Escort Ships", "SK C/32 naval gun", 0.05, 50, random_number(5, 10), random_number(1, 3)},
  {"E_E", "Japanese Kaibokan", "(4.7 inch) naval guns", 0.04, 70, random_number(5, 15), random_number(1, 3)},
};
 
  char exit;
  
  do{

    char choice;
    
    printf("Main Menu\n");
    printf("\t1.Start Simulation\n");
    printf("\t2.View Instructions\n");
    printf("\t3.Simulation Statistics\n");
    printf("\t4.Exit\n");
  
    printf("\nEnter your choice: ");
    scanf(" %c", &choice);
  
    switch (choice){
      
      case '1':
        battle(battleships, escortships);
        break;

      case '2':
        instructions(battleships, escortships);
        break;

      case '3':
          system("gedit Battle_data.txt");
        break;

      case '4':
        printf("Exiting the program.......\n");
        return 0;
        
        break;

      default:
        printf("Invalid choice, Please try again.\n");
        
        break;
    }
  
    while((getchar()) != '\n');
    
    printf("\nDo you want to go back to the main menu? (Y/N): ");
    scanf(" %c", &exit);
  }while (exit == 'Y' || exit == 'y');

  printf("\nExiting the program.......\n");
  return 0;  
}