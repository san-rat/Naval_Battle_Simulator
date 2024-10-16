/* This program is a Advanced Naval Battle simulator program. 

 You will have to give the dementions of the battlefield, the battleship you choose to fight with, where to place the battleship within the battlefield, and the number of enemy escort ships within the battlefield.

 Also, you must provide us the number of times you want the Battleship to move within the battlefield.

 The velocities of the battleship as well as the escortships will be randomly generated within the limits and the escort ships will also be randomly generated.
 The escort ship type, position, velocities, angles will be randomly generated by the program.

 In this program you will see where the ships are placed within the battlefield, and you will find the destroyed ships on the battlefield as well.
 Destroyed battleship will be seen as "X", and the destroyed escort ships will be seen as "x".

 The battleship will randomly move within the battlefield according to the number of times you have given the ship to move and the escort ships are stationary targets.

 The battleship's gun will get jammed during any time of simulation (it time the guns get jammed are randomly chosen within the given number of times the battleship will move in the battlefield) and after jammed the guns shooting range is reduced in an angle randaomly chosen within 0 and 30 degrees.

 Battleships can destroy a escortship in one hit, but the escort ships has a factor called "impact power" which can partially destroy a battleship.
 So, only the accumilated damage done by the escort ships reaches a 100%, will the battleship be destroyed.

 A file (Battle_log.txt) will be created in the same directory as the program and the file will have all the information about the battle after the simulation has been run once.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define pi 3.141592653589793238462643383279502884197169399375105820
#define best_angle 45
#define gravity 9.80665

//Structure of a Battleship.
typedef struct {           
  char ship_name[20]; 
  char notation[2];  
  char gun[30]; 
  int max_velocity;  
  int x;  
  int y;
  int min_angle;
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

//Structure of the coordinates taken by the battleship.
typedef struct{
int x;
int y;
} b_path;

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
int in_range(escort_ship Eships, battle_ship Bship, b_path path){

  double distance = sqrt(pow(path.x - Eships.x, 2) + pow(path.y - Eships.y, 2));
  return (distance <= range(Bship.max_velocity, best_angle));
}

//Function to calculate the distance betweent two ships.
float short_distance(escort_ship Eships, b_path path){
  
  return sqrt(pow(path.x - Eships.x, 2) + pow(path.y - Eships.y, 2));
}

//Function where the battleships attack the escortships.
void b_attack(escort_ship Eships[], int eships, battle_ship Bship, int dx, int dy, b_path path){

  for(int i = 0; i < eships; i++){
  
    if(in_range(Eships[i], Bship, path)){
    
      Eships[i].status = 1;
    }
  }
}

//Function to calculate the the attack range of the battleship after jamming.
void b_jammed(escort_ship E_ships[], int e_ships, battle_ship B_ship, int dx, int dy, b_path path){

  float upper_limit = 0, lower_limit = 0, b_to_e_distance = 0;

  for(int i = 0; i < e_ships; i++){
    
    if(E_ships[i].status == 1){
    
      upper_limit = range(B_ship.max_velocity, best_angle);
      lower_limit = range(1, B_ship.min_angle);

      b_to_e_distance = short_distance(E_ships[i], path);

      if(b_to_e_distance <= upper_limit && b_to_e_distance >= lower_limit){
      
        E_ships[i].status = 0;
      }
    }
  }
}

//Function where the escortships attack the battleships.
void e_attack(escort_ship Eships[], int eships, battle_ship Bship, float *damage, b_path path){

  float upper_limit = 0, lower_limit = 0, e_to_b_distance = 0;

  for(int i = 0; i < eships; i++){

    if(Eships[i].status != 1){

      if(Eships[i].max_angle >= best_angle){
        
        upper_limit = range(Eships[i].max_velocity, best_angle);
      }
      else{
        
        upper_limit = range(Eships[i].max_velocity, Eships[i].max_angle);
      }

      lower_limit = range(Eships[i].min_velocity, Eships[i].min_angle);
      e_to_b_distance = short_distance(Eships[i], path);

      if(e_to_b_distance <= upper_limit && e_to_b_distance >= lower_limit){
        
        *damage = *damage + Eships[i].impact_power;
      }
    }
  }
}

//Function to calculate the maximum velocity of all the escortships.
void calculateMaxV(escort_ship escortships[], battle_ship Bship){
    
  for(int i = 0; i < 5; i++){
  
    if(i == 0) {
    
      escortships[i].max_velocity = 1.2 * Bship.max_velocity;  //Maximum velocity of escortship E_A.  
    } else {
           
      escortships[i].max_velocity = random_number(1,Bship.max_velocity-1);  //Make the Maximum velocity of the escortships lower that the battleships maximum velocity.  
    }
  }
}

//Function to print the battlefield before the battle.(canvas)
void battlefield_before(escort_ship E_ships[], battle_ship B_ship, int dx, int dy, int e_ships, int **canvas, int B_status, b_path path[], int points){
  
  for(int i = 0; i < dy; i++){
  
    for(int j = 0; j < dx; j++){
    
      canvas[i][j] = 0; 
    }
  }

  canvas[dy - B_ship.y][B_ship.x - 1] = -1;

  for(int j = 0; j < e_ships; j++){
    
    canvas[dy - E_ships[j].y][E_ships[j].x - 1] = E_ships[j].index;
  }

  for(int p = 1; p < points; p++){
    
    canvas[dy - path[p].y][path[p].x - 1] = -2;
  }

  printf("\n");  //Printing the battlefield.
  
  for (int i = dy - 1; i >= 0; i--) {
  
    for (int j = 0; j < dx; j++) {
    
      if (canvas[i][j] == 0) {
      
        printf(". ");  
      } else if (canvas[i][j] == -1) {

        printf("B ");  
      } else if (canvas[i][j] == -2) {

        printf("p ");
      } else if (canvas[i][j] == -3) {
    
        printf("x ");
      } else if (canvas[i][j] >= 10) {
    
        printf("%d", canvas[i][j]);
      } else {
    
        printf("%d ", canvas[i][j]);
      }
    }
    printf("\n");
  }
}

//Function to print the battlefield after the battle.
void battlefield_after(escort_ship E_ships[], battle_ship B_ship, int dx, int dy, int e_ships, int **canvas, int B_status, b_path path[], int points, int count, int b_x, int b_y){
  
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

  printf("\n"); //Printing the battlefield.
    
  for (int i = dy - 1; i >= 0; i--) {
  
    for (int j = 0; j < dx; j++) {
    
      if (canvas[i][j] == 0) {
      
        printf(". ");
      } else if (canvas[i][j] == -1) {
  
        printf("X ");
      } else if (canvas[i][j] == -2) {
      
        printf("p ");
      } else if (canvas[i][j] == -3) {
      
        printf("x ");
      } else if (canvas[i][j] >= 10) {  
      
        printf("%d", canvas[i][j]);
      } else {
      
        printf("%d ", canvas[i][j]);
      }  
    }   
    printf("\n");
  }
}

//Function on how the battleship will move on the canvas one point to another.
void b_ship_path(b_path path[], escort_ship E_ships[],int e_ships, battle_ship B_ship, int dx, int dy, int points){

  path[0].x = B_ship.x;
  path[0].y = B_ship.y;

  for(int i =1; i < points; i++){
    
    int new_x, new_y;
    bool overlap = false;

    do{
      new_x = random_number(path[i - 1].x - 1, path[i -1].x + 1);
    
      while(new_x == B_ship.x){
      
        new_x = random_number(path[i - 1].x - 1, path[i -1].x + 1);
      }

      new_y = random_number(path[i - 1].y - 1, path[i - 1].y + 1);
      
      while(new_y == B_ship.y){
      
        new_y = random_number(path[i - 1].y - 1, path[i - 1].y + 1);
      }

      for(int j = 0; j < e_ships; j++){
        
        if(new_x == E_ships[j].x && new_y == E_ships[j].y){
        
          overlap = true;
          break;
        }
      }
    }while(overlap || new_x <= 0 || new_y <= 0 || new_x > dx || new_y > dy);

    path[i].x = new_x;
    path[i].y = new_y;
  }
}

//Function that simulate the full battle.
void battle(battle_ship battleships[], escort_ship escortships[]){

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
   
    printf("%-23s %-10s %-25s %-16d\n", battleships[i].ship_name, battleships[i].notation, battleships[i].gun, battleships[i].max_velocity);
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
    
    printf("\nCongradulations!");
    printf("\nYou selected %s Battleship\n", battleships[0].ship_name);
    
    B_ship = battleships[0];  //Copying the data of the selected battleship to the B_ship array.(variable)
    
    calculateMaxV(escortships, B_ship);  //Calling the function to calculate the Maximum Velocity of all the Escort Ships.
  }

  if (choice == 'm' || choice == 'M') {
    
    printf("\nCongradulations!");
    printf("\nYou selected %s Battleship\n", battleships[1].ship_name);
    
    B_ship = battleships[1];
    
    calculateMaxV(escortships, B_ship);
  }

  if (choice == 'r' || choice == 'R') {
    
    printf("\nCongradulations!");
    printf("\nYou selected %s Battleship\n", battleships[2].ship_name);
    
    B_ship = battleships[2];
    
    calculateMaxV(escortships, B_ship);
  }

  if (choice == 's' || choice == 'S') {
    
    printf("\nCongradulations!");
    printf("\nYou selected %s Battleship\n", battleships[3].ship_name);
    
    B_ship = battleships[3];
    
    calculateMaxV(escortships, B_ship);
  }

  printf("\nInput coordinates for your Battleship: ");  //Input the coodinates of the battleship.
  scanf("%d %d", &B_ship.x, &B_ship.y);

  while(!(B_ship.x <= dx && B_ship.x >= 1 && B_ship.y <= dy && B_ship.y >= 1)){
    
    printf("\nThe Battleship is not inside the battlefield.\nPlease enter valid coordinates for you Battleship: ");
    scanf( "%d %d", &B_ship.x, &B_ship.y);
  }

  int points;
  printf("\nEnter the number of times the Battleship moves on the battlefield: ");
  scanf("%d", &points);
  
  int e_ships;
  printf("\nEnter number of Escort ships: ");  //Input the number of Escort ships.
  scanf("%d",&e_ships);

  while(!(e_ships >= 1 && e_ships < dx * dy - points)){ 
    printf("\nThe number of Escort ships must be between 1 and %d.\nPlease enter a valid number: ", dx * dy);
    scanf(" %d", &e_ships); //Made sure to limit the number of escort ships to the space availablie in the battlefield.
  }

  escort_ship E_ships[e_ships];
  b_path path[points];

  for(int j = 0; j < e_ships; j++){
    
    int index_no = random_number(0,4);   //Generating a number between 0 and 4.
    E_ships[j] = escortships[index_no];  //Copying attributes to the arrays of structures.
    
    E_ships[j].x = random_number(1,dx-1);
    E_ships[j].y = random_number(1,dy-1); //Randomly generating the coordinates in the battlefield.
   
    E_ships[j].index = j+1;  //Assigning an index number to all the escort ships.
  }

  int **canvas = (int **)malloc(dy * sizeof(int *));
  
  if (canvas == NULL) {
  
    printf("Memory allocation failed\n");
    return; 
  }
  
  for(int i = 0; i < dy; i++){
  
    canvas[i] = (int *)malloc(dx * sizeof(int));
    
    if (canvas[i] == NULL) {
    
      printf("Memory allocation failed\n");
      return; 
    }
  }

  for(int i = 0; i < e_ships; i++){
   
    E_ships[i].max_angle = E_ships[i].min_angle + E_ships[i].angle_range; //Calculating the maximum angle of the escort ships.
  }

  int destroyed_e_ships = 0;
  
  int B_status = 0;
  
  b_ship_path(path, E_ships, e_ships, B_ship, dx, dy, points);
  
  printf("\nBattlefield before battle.");
  
  battlefield_before(E_ships, B_ship, dx, dy, e_ships, canvas, B_status, path, points);
  
  int b_x, b_y, jammed = 0, count = 0;

  jammed = random_number(1, points - 1);
  
  float b_damage = 0;

  for(int i = 0; i <jammed; i++){

    e_attack(E_ships, e_ships, B_ship, &b_damage, path[i]);
    b_attack(E_ships, e_ships, B_ship, dx, dy, path[i]);

    b_x = path[i].x;
    b_y = path[i].y;

    count = i + 1;

    if(b_damage >= 1){
      
      B_status = 1;
      break;
    }
  }

  for(int i = jammed; i < points; i++){

    B_ship.min_angle  = random_number(1, 29);

    e_attack(E_ships, e_ships, B_ship, &b_damage, path[i]);
    b_jammed(E_ships, e_ships, B_ship, dx, dy, path[i]);

    b_x = path[i].x;
    b_y = path[i].y;

    count = i +1;

    if(b_damage >= 1){

      B_status = 1;
      break;
    }
  }

  for(int i = 0; i < e_ships; i++){
    
    if(E_ships[i].status == 1){
    
      destroyed_e_ships++;
    }
  }

  printf("\nBattlefield after battle.");
  printf("\nThe Battleship's gun jammed on the %dth point.", jammed);

  battlefield_after(E_ships, B_ship, dx, dy, e_ships, canvas, B_status, path, points, count, b_x, b_y);
  
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
  fprintf(fileptr, "%-6c %-18s %-17d\n", choice, Bcoordinates, B_ship.max_velocity);
  fprintf(fileptr, "\nEscort Ships:\n");
  fprintf(fileptr, "%-6s %-4s %-10s %-7s %-7s %-7s %-7s\n", "Type","ID","Position","Min V","Max V","Min θ","Max θ");
  
  for(int k = 0; k < e_ships; k++){
  
    fprintf(fileptr,"%-6s %-4d %-10s %-7d %-7.2f %-7d %-7d\n", E_ships[k].notation, E_ships[k].index, Ecoordinates[k], E_ships[k].min_velocity, E_ships[k].max_velocity, E_ships[k].min_angle, E_ships[k].max_angle);  
  }
  
  fclose(fileptr);  //Closing the file.
}

//Function for the 'Instructions' part of the menu.
void instructions(battle_ship battleships[], escort_ship escortships[]){


  printf("\nBattleship Information:\n");  //Printing all the data of the Battleships.  
  printf("\n%-23s %-10s %-25s %-16s\n", "Battleship Name", "Notation", "Gun Name", "Maximum Velocity");

  for (int i = 0; i < 4; i++) {

    printf("%-23s %-10s %-25s %-16d\n", battleships[i].ship_name, battleships[i].notation, battleships[i].gun, battleships[i].max_velocity);
  }

  printf("\nEscort Ship Information:\n");  //Printing all the data of the escort ships.
  printf("\n%-15s %-25s %-27s %-15s %-13s %-15s %-17s\n", "Type Notation", "Type Name", "Gun Name", "Impact Power", "Angle Range", "Minimum Angle", "Minimum Velocity");

  for (int i = 0; i < 5; i++) {

    printf("%-15s %-25s %-27s %-15.2f %-13d %-15d %-17d\n", escortships[i].notation, escortships[i].ship_name, escortships[i].gun, escortships[i].impact_power, escortships[i].angle_range, escortships[i].min_angle, escortships[i].min_velocity);
  }

  printf("\nFirst provide the dimension of the battlefield. (Please note that the battlefield must be a square)\n\nThen, choose a type of Battleship from the catalogue of batttleships given to you and enter the 'Notation' of the chosen Battleship.\n\nAfter that, provide the coordinates of the Battleship within the battlefield as well as the number of times you want the Battleship to move on the battlefield.\n\nAnd finally, enter the number of Escort ships you want to have in the simulation.\n\nThe type, position and the maximum velocity of each Escort ships will be randomly generated.\n\nThe Battleship will move within the battlefield randomly and can move once to a point around the Battleship. (Just like how the King moves in chess.)\n\nPlease note that the maximum velocity of the E_A Escort ships will be 1.2 times the maximum velocity of the Battleship.\n\nThe maximum velocity of the other randomly generated Escort ships will be lower than the maximum velocity of the Battleship.\n\nPlease note that the Battleships guns will get jammed some time randomly during the simulation. (randomly within the number of points.)\n\nThe Battleship can destroy a escortship by attacking the ship once, but the escortships has an impact power on how much damage an escort ship can do to a battleship in one shell.\nOnly accumilated power of the escortships can destroy the battleship.\n\n");  
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