#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    unsigned int card_id;
} CARD_DESCRIPTOR;


enum    { ERROR_NONE, ERROR_NO_CARDS, ERROR_NO_MEMORY_AVAILABLE };
enum    { FALSE, TRUE };


//------ Function Prototypes -------------------------------------------------------------

void            initTable(void);
void            initHand(void);
void            doRound(void);
unsigned int    doOriginalOrderCheck(void);


//------  GLOBAL VARIABLES  --------------------------------------------------------------

CARD_DESCRIPTOR    *hand_base_ptr, *hand_ptr, *table_base_ptr, *table_ptr;
unsigned int        round_counter = 0, total_number_of_cards;

/*----------------------------------------------------------------------------------------
The main function executes the algorithm outlined in the directions. In actual production Code
we could add things like additional checks to the user input (ie user enters 'A'), bound checks
on number of cards, add additional error flags etc.  For the scope of this exercise we assume
the input will be valid and within a reasonable number of cards that you can actuall hold in your
hands. The scope of this exercise is to showcase the algorithm and not to handle every possible
error scenerio or corner case.
----------------------------------------------------------------------------------------*/

int main()
{
    unsigned int error_code = ERROR_NONE;


    printf("\nHow Many Cards in the Deck?  ");
    scanf("%d", &total_number_of_cards );

    switch( total_number_of_cards ) 
    {

        case 0:
            error_code = ERROR_NO_CARDS;
            goto done;

        case 1:
            round_counter = 1;
            goto done;

        default:                // Handle all real cases
            break;

    }

    hand_base_ptr  = (CARD_DESCRIPTOR *)malloc( sizeof(CARD_DESCRIPTOR) * total_number_of_cards );  // Allocate hand deck
    table_base_ptr = (CARD_DESCRIPTOR *)malloc( sizeof(CARD_DESCRIPTOR) * total_number_of_cards );  // Allocate table deck


    if( (hand_base_ptr == NULL) || (table_base_ptr == NULL) )   // Check for allocation errors
    {
        error_code = ERROR_NO_MEMORY_AVAILABLE;
        goto done;
    }

    initTable();                

    while( 1 ) 
    {

        doRound();

        if( doOriginalOrderCheck() == TRUE )
            break;

    }

done:

    free( hand_base_ptr );
    free( table_base_ptr );

    switch( error_code ) 
    {

        case ERROR_NONE:
            printf("\nNumber of Rounds Required = %d\n", round_counter);
            break;

        case ERROR_NO_CARDS:
            printf("\nERROR: No Cards in the Deck!\n");
            break;

        case ERROR_NO_MEMORY_AVAILABLE:
            printf("\nERROR: Not Enough Memory Available\n");
            break;

        default:
            printf("\nERROR: Unknown Error Code\n");
            break;

    }

    return 0;
}

//------- Initialize Structure for the Initial Deck on the Table ------------------------------

void initTable(void)
{
    unsigned int i;


    table_ptr = table_base_ptr;     // Copy address of base card

    for( i = 1; i <= total_number_of_cards; i++ ) // Set card order number
    {
        table_ptr->card_id = i;
        table_ptr++;
    }

    table_ptr = table_base_ptr;

    return;
}



//------- Initialize Hand from the Deck on Table(move table deck to hand) ------------------------

void initHand(void)
{
    unsigned int i;

    hand_ptr  = hand_base_ptr;
    table_ptr = table_base_ptr;

    for( i = 0; i < total_number_of_cards; i++ )    // Copy table deck order to hand deck
    {
        hand_ptr->card_id = table_ptr->card_id;
        hand_ptr++;
        table_ptr++;
    }

    hand_ptr  = hand_base_ptr;
    table_ptr = table_base_ptr;

    return;
}


//------------- Do a "Round" -----------------------------------------
// The Pack of Cards is held in the hands Face-Down.
// Cards placed on the Table are placed Face-Down.
// Cards brought from the Top of the Pack to the Bottom of the Pack always remain Face-Down.
// The Pack on the Table is Picked-Up together and Held in the Hands Face-Down
// when dealing out each "Round".

void doRound(void)
{
    unsigned int i, moved = 0, alternate = TRUE;

    initHand();         // Copy table deck to hand

    table_ptr += total_number_of_cards - 1; // reverse order on table deck

    while( moved < total_number_of_cards ) // Loop through deck on hand until all cards have been moved to table
    {
        // There is no need to put every other card under the deck, this process will leave cards in same order as moving every other card to bottom
        hand_ptr = hand_base_ptr;

        for( i = 0; i < total_number_of_cards; i++ ) 
        {
            if( hand_ptr->card_id )     // Moved cards are flagged by setting card_id to 0
            {
                if( alternate == TRUE ) 
                {
                    alternate = FALSE;
                    table_ptr->card_id = hand_ptr->card_id;
                    table_ptr--;
                    moved++;
                    hand_ptr->card_id = 0; //
                } 
                else
                    alternate = TRUE;
            }
            hand_ptr++;
        }
    }

    round_counter++;

    return;
}

//------- Check if cards are in original order ------------------------

unsigned int doOriginalOrderCheck(void)
{
    unsigned int i, result = TRUE;

    table_ptr = table_base_ptr;

    for( i = 1; i <= total_number_of_cards; i++ )   // If card id's are not sequential then deck is not in order
    {
        if( table_ptr->card_id != i ) 
        {
            result = FALSE;
            break;
        } 
        else 
            table_ptr++;
    }

    return( result );
}




    