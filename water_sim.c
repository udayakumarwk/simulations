#include<stdio.h>
#include<SDL2/SDL.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>


#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600
#define CELL_SIZE 5
#define LINE_SIZE 1
#define ROWS SCREEN_HEIGHT/CELL_SIZE
#define COLUMNS SCREEN_WIDTH/CELL_SIZE
#define CELL_LENGTH COLUMNS*ROWS

#define NO_DIRECTION -1
#define RIGHT_DIRECTION 0
#define LEFT_DIRECTION 1

//COLORS
#define COLOR_BLUE 0x416bdfFF
#define GRID_COLOR 0x80808080 
#define COLOR_GREY  0x80808080 
#define COLOR_BLACK 0x0000000 
#define COLOR_WHITE 0xFFFFFFFF
//CELL TYPES
#define BLANK_CELL 0
#define WATER_CELL 1
#define STATIC_CELL 2

typedef struct Cell {
    int x;
    int y;
    bool isvalid;
    int cell_direction;
    int cell_type;
    int count;
}Cell;

//array of all cell data

Cell cells[ROWS][COLUMNS];
int cell_count = 0;

//globals
bool should_running = true;
int draw_mode = WATER_CELL;

void init_cells(){

    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLUMNS; j++)
        {
            cells[i][j] = (Cell){0};
            cells[i][j].cell_type = BLANK_CELL;
            cells[i][j].cell_direction = NO_DIRECTION;
        }
    }
    printf("CELL COLUMNS = %d\n", ROWS);
    printf("CELL ROWS = %d\n", COLUMNS);
    printf("CELL SIZE = %d\n", CELL_SIZE);
    printf("CELL_LENGTH = %d\n", CELL_LENGTH);
}
void handle_events(SDL_Surface* surface, SDL_Event *event)
{   while(SDL_PollEvent(event))
    {
        switch(event->type)
        {
            case SDL_QUIT:
                {
                    should_running = false; 
                    break;
                }
            case SDL_MOUSEMOTION:
                {
                    if(event->motion.state !=0)
                    {
                        Cell cell = {0};
                        int x = (event->motion.x / CELL_SIZE); 
                        int y = (event->motion.y /CELL_SIZE);
                        cell.x = x * CELL_SIZE;
                        cell.y = y * CELL_SIZE;
                        cell.isvalid = true;
                        cell.cell_type = draw_mode;
                        //printf("x=%d, y=%d\n", cell.x / CELL_SIZE, cell.y / CELL_SIZE);
                        //printf("array index = %d\n",x + COLUMNS * y);
                        cells[y][x] = cell;
                     }
                    break;
                }
            case SDL_KEYDOWN:
                {
                     //printf("key %c pressed\n",event->key.keysym.sym);

                    if(event->key.keysym.sym == SDLK_SPACE)
                    {
                        //printf("space pressed\n");
                        if(draw_mode == WATER_CELL){
                            draw_mode = STATIC_CELL;
                            printf("Mode changed to STATIC CELL\n");
                        }else{
                            draw_mode = WATER_CELL;
                            printf("Mode changed to WATER CELL\n");
                        }
                    }
                    break;
                }
            default:
                {
                
                }
        }
    }

}

void draw_cells(SDL_Surface* surface)
{   
    for(int i=0; i<ROWS; i++)
    {
        for(int j=0; j<COLUMNS; j++)
        {
            Cell cell = cells[i][j];
            if(!cell.isvalid)
                continue;
            SDL_Rect rect = (SDL_Rect) {.x=cell.x, .y=cell.y, .h=CELL_SIZE, .w=CELL_SIZE};
            if(cell.cell_type == WATER_CELL)
            {
                SDL_FillRect(surface, &rect, COLOR_BLUE); 
            }
            else if(cell.cell_type == STATIC_CELL)
            {
                SDL_FillRect(surface, &rect, COLOR_WHITE); 
            }
            else
            {
                SDL_FillRect(surface, &rect, COLOR_BLACK); 
            }
        }

    }

}

void draw_grids(SDL_Surface* surface)
{
   for (int i=0; i<ROWS; i++)
   {
        SDL_Rect rect = (SDL_Rect) { .x=0, .y=i*CELL_SIZE, .w=SCREEN_WIDTH, .h=LINE_SIZE };
        SDL_FillRect(surface, &rect, GRID_COLOR);
        //printf("x = %d, y=%d, w = %d, h = %d\n",rect.x, rect.y, rect.w, rect.h);
    }
 for (int i=0; i<COLUMNS; i++)
   {
        SDL_Rect rect = (SDL_Rect) { .x=i*CELL_SIZE, .y=0, .w=LINE_SIZE, .h=SCREEN_HEIGHT};
        SDL_FillRect(surface, &rect, GRID_COLOR);
        //printf("x = %d, y=%d, w = %d, h = %d\n",rect.x, rect.y, rect.w, rect.h);

   }

}

void update_physics(){

    for(int i=ROWS-1; i>=0; i--)
    {
        for(int j=COLUMNS-1; j>=0; j--)
        {
            //printf("i=%d, j=%d\n",i,j);
            Cell cell = cells[i][j];
            if(cell.x >= SCREEN_WIDTH || cell.y >= SCREEN_HEIGHT )
            {
                //printf("i=%d, j=%d, index=%d\n",i,j,index);
                //printf("cellx=%d, celly=%d\n",cell.x, cell.y);
                continue;
            }
            if(cell.cell_type == WATER_CELL)
            {
                if(cell.cell_direction == NO_DIRECTION)
                {
                    int random_bit = rand() % 2;
                    cell.cell_direction = random_bit;
                
                }
                if(cell.cell_direction == RIGHT_DIRECTION)
                     {
                        if(cells[i][j+1].cell_type == WATER_CELL || cells[i][j+1].cell_type == STATIC_CELL)
                        {
                            cell.cell_direction = LEFT_DIRECTION;
                        }
                     }
                else if(cell.cell_direction == LEFT_DIRECTION && (cells[i][j+1].cell_type == WATER_CELL || cells[i][j+1].cell_type == STATIC_CELL)
 )
                {
                    cell.cell_direction = RIGHT_DIRECTION;
                }

                int index_t = i+1;
                if(index_t >= ROWS)
                {
                    continue;
                }
                //if(cells[index_t][j].cell_type == STATIC_CELL)
                //{
                //    printf("Below cell is static cell\n");
                //    continue;
                //}

                //if below cell is blank
                if(cells[index_t][j].cell_type == BLANK_CELL)
                {
                     cell.y = cell.y + CELL_SIZE;
                     cells[index_t][j] = cell;
                     Cell cell_t = {0};
                     cell_t.cell_type = BLANK_CELL;
                     cells[i][j] = cell_t;


                } 
                //if below cell is already occupied
                else if(cells[index_t][j].cell_type == WATER_CELL || cells[index_t][j].cell_type == STATIC_CELL )
                {
                    //if bottom right cell is blank
                   if(j+1 < COLUMNS && cells[index_t][j+1].cell_type == BLANK_CELL)
                   {
                        cell.y = cell.y + CELL_SIZE;
                        cell.x = cell.x + CELL_SIZE;
                        cells[index_t][j+1] = cell;
                        Cell cell_t = {0};
                        cell_t.cell_type = BLANK_CELL;
                        cells[i][j] = cell_t;
                   }
                   //if bottom left cell is blank
                   else if(j-1>=0 && cells[index_t][j-1].cell_type == BLANK_CELL){
                        cell.y = cell.y + CELL_SIZE;
                        cell.x = cell.x - CELL_SIZE;
                        cells[index_t][j-1] = cell;
                        Cell cell_t = {0};
                        cell_t.cell_type = BLANK_CELL;
                        cells[i][j] = cell_t;
                   }
                   //if left cell and right cell is blank
                 else if(j+1 < COLUMNS && j-1>=0 && cells[i][j-1].cell_type == BLANK_CELL && cells[i][j+1].cell_type == BLANK_CELL){
                     
                     
                     if(cell.cell_direction == RIGHT_DIRECTION)
                     {
                        //printf("Moving cell right\n");
                        cell.x = cell.x + CELL_SIZE;
                        cells[i][j+1] = cell;
                     }
                     else
                     {
                        //printf("Moving cell left\n");
                        cell.x = cell.x - CELL_SIZE;
                        cells[i][j-1] = cell;
                     }

                        Cell cell_t = {0};
                        cell_t.cell_type = BLANK_CELL;
                        cells[i][j] = cell_t;

                }

                    //if right cell is blank
                    else if(j+1 < COLUMNS && cells[i][j+1].cell_type == BLANK_CELL){
                        //cell.y = cell.y + CELL_SIZE;
                        cell.x = cell.x + CELL_SIZE;
                        cells[i][j+1] = cell;
                        Cell cell_t = {0};
                        cell_t.cell_type = BLANK_CELL;
                        cells[i][j] = cell_t;

                }
                    //if left cell is blank
                    else if(j-1 >=0 && cells[i][j-1].cell_type == BLANK_CELL){
                        //cell.y = cell.y + CELL_SIZE;
                        cell.x = cell.x - CELL_SIZE;
                        cells[i][j-1] = cell;
                        Cell cell_t = {0};
                        cell_t.cell_type = BLANK_CELL;
                        cells[i][j] = cell_t;

                }

                }
                
                 //printf("i=%d, j=%d, index=%d, index_t=%d\n",i,j,index,index_t);
                 
           }
            
        }
    }
}

int main()
{

    srand(time(NULL));
    printf("Hello World!\n");
    SDL_Init(SDL_INIT_VIDEO);

    
    SDL_Window* window = SDL_CreateWindow("SDL2", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(window == NULL) 
    { 
        printf("Not able to create window"); 
        return -1; 
    }
    init_cells();
    SDL_Rect rect = (SDL_Rect) {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT}; 
    SDL_Surface *surface = SDL_GetWindowSurface(window); 
    SDL_FillRect(surface, &rect, 0x00000000); 

    //DRAW GRIDS
    draw_grids(surface);
    SDL_UpdateWindowSurface(window);

    //MAIN LOOP
    while(should_running)
    {
        rect = (SDL_Rect) {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT}; 
        surface = SDL_GetWindowSurface(window); 
        SDL_FillRect(surface, &rect, 0x00000000); 
       //draw_grids(surface);
        SDL_Event event;
        handle_events(surface, &event);
        draw_cells(surface);
        SDL_UpdateWindowSurface(window);
        update_physics();
        SDL_Delay(15);
    }
    SDL_Quit();
    return 0;

}
