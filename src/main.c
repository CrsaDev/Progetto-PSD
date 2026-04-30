#include <stdio.h>
#include <stdlib.h>

#include "date.h"
#include "report.h"
#include "gestionale.h"

void print_menu() {
    printf("=========================================\n");
    printf("     PER SALVARE I DATI SCEGLIERE 0    \n");
    printf("=========================================\n");


    printf("=========================================\n");
    printf("       SISTEMA GESTIONE SEGNALAZIONI       \n");
    printf("=========================================\n");
    printf("1. Visualizza tutte le segnalazioni\n");
    printf("2. Cerca segnalazione per ID\n");
    printf("3. Visualizza segnalazione piu' URGENTE\n");
    printf("4. Aggiorna stato di una segnalazione\n");
    printf("5. Visualizza Dashboard Statistiche\n");
    printf("6. Inserisci NUOVA segnalazione\n");
    printf("0. Salva ed esci dal programma\n");
    printf("=========================================\n");
    printf("Scelta: ");
}

/* Used to remove useless imput from the buffer */
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    system("cls || clear");
    printf("--- Inizializzazione Sistema ---\n");

    gestionale g = gestionale_create();
    if (!g) {
        printf("Errore critico: Impossibile avviare il gestionale.\n");
        return -1;
    }

    int choice;
    do {
        print_menu();
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            choice = -1; 
        }

        switch (choice) {
        case 1: {
                int filter_choice_cat, filter_choice_stat;
                int filter_cat, filter_stat;

                printf("\n--- Filtri di Visualizzazione ---\n");
                
                printf("Seleziona la Categoria:\n");
                printf("0: Tutte le categorie\n");
                printf("1: Solo (Strade/Buche)\n");
                printf("2: Solo (Illuminazione)\n");
                printf("3: Solo (Rifiuti)\n");
                printf("4: Solo (Guasti generici)\n");
                printf("Scelta: ");

                if (scanf("%d", &filter_choice_cat) != 1 || filter_choice_cat < 0 || filter_choice_cat > 4) {
                    printf("\nErrore: Categoria non valida. Visualizzazione annullata.\n");
                    clear_input_buffer();
                    break;
                }

                // One-to-one mapping of the categories
                switch (filter_choice_cat) {
                    case 0: filter_cat = -1;        break; 
                    case 1: filter_cat = STREET;    break; 
                    case 2: filter_cat = LIGHTNING; break; 
                    case 3: filter_cat = WASTE;     break; 
                    case 4: filter_cat = FAULT;     break; 
                }

                printf("\nSeleziona lo Stato:\n");
                printf("0: Tutti gli stati\n");
                printf("1: Solo PENDING (In attesa)\n");
                printf("2: Solo IN_PROGRESS (In lavorazione)\n");
                printf("3: Solo RESOLVED (Chiuse)\n");
                printf("Scelta: ");

                if (scanf("%d", &filter_choice_stat) != 1 || filter_choice_stat < 0 || filter_choice_stat > 3) {
                    printf("\nErrore: Stato non valido. Visualizzazione annullata.\n");
                    clear_input_buffer(); 
                    break; 
                }

                // One-to-one mapping of the statuses
                switch (filter_choice_stat) {
                    case 0: filter_stat = -1;          break;
                    case 1: filter_stat = PENDING;     break;
                    case 2: filter_stat = IN_PROGRESS; break;
                    case 3: filter_stat = RESOLVED;    break;
                }

                gestionale_view_reports(g, filter_cat, filter_stat);
                break;
            }

            case 2: {
                int search_id;
                printf("Inserisci l'ID della segnalazione da cercare: ");
                scanf("%d", &search_id);
                gestionale_find_report(g, search_id);
                break;
            }

            case 3:
                gestionale_view_urgent(g);
                break;

            case 4: {
                int update_id, new_status;
                printf("Inserisci l'ID da aggiornare: ");
                scanf("%d", &update_id);
                printf("Stato (0: PENDING, 1: IN_PROGRESS, 2: RESOLVED): ");
                scanf("%d", &new_status);
                
                if (new_status >= 0 && new_status <= 2) {
                    gestionale_update_report_status(g, update_id, (status)new_status);
                } else {
                    printf("Stato non valido!\n");
                }
                break;
            }

            case 5:
                gestionale_view_final_report(g);
                break;

            case 6: { 
                int id, prio, cat_int, stat_int;
                char citizen[50], desc[100];
                int dd, mm, yy;

                printf("\n--- Inserimento Nuovo Report ---\n");
                
                printf("ID Segnalazione (Numero maggiore di 0): "); 
                if (scanf("%d", &id) != 1 || id <= 0) {
                    printf("\nErrore: L'ID deve essere un numero intero positivo.\n");
                    printf("Inserimento annullato.\n");
                    break;
                }
                
                clear_input_buffer(); 
                printf("Nome Cittadino: "); 
                scanf(" %[^\n]s", citizen); 

                printf("Categoria (0:STRADE, 1:ILLUMINAZIONE, 2:RIFIUTI, 3:MALFUNZIONAMENTI): ");
                if (scanf("%d", &cat_int) != 1 || cat_int < 0 || cat_int > 3) {
                    printf("\nErrore: Categoria non valida. Deve essere un numero tra 0 e 3.\n");
                    printf("Inserimento annullato.\n");
                    break;
                }

                clear_input_buffer();
                printf("Descrizione: "); 
                scanf(" %[^\n]s", desc);

                printf("Data (formato GG MM AAAA): ");
                if (scanf("%d %d %d", &dd, &mm, &yy) != 3) {
                    printf("\nErrore: Formato data non valido.\n");
                    printf("Inserimento annullato.\n");
                    break;
                }

                date new_date = date_create(dd, mm, yy);
                if (!new_date) {
                    printf("\nErrore: La data inserita (%02d/%02d/%04d) non esiste o non e' valida!\n", dd, mm, yy);
                    printf("Inserimento annullato.\n");
                    break; 
                }

                printf("Priorita' (Valore numerico maggiore di 0): ");
                if (scanf("%d", &prio) != 1 || prio <= 0) {
                    printf("\nErrore: La priorita' deve essere un numero intero maggiore di 0.\n");
                    printf("Inserimento annullato.\n");
                    date_destroy(new_date); // Prevents memory leaks
                    break;
                }

                printf("Stato (0:PENDING, 1:IN_PROGRESS, 2:RESOLVED): ");
                if (scanf("%d", &stat_int) != 1 || stat_int < 0 || stat_int > 2) {
                    printf("\nErrore: Stato non valido. Deve essere un numero tra 0 e 2.\n");
                    printf("Inserimento annullato.\n");
                    date_destroy(new_date); 
                    break;
                }

                report new_report = report_create(id, citizen, (category)cat_int, desc, new_date, prio, (status)stat_int);
                if (gestionale_add_report(g, new_report)) {
                    printf("\nSuccesso: Segnalazione inserita correttamente!\n");
                } else {
                    printf("\nErrore: Impossibile inserire la segnalazione. (ID Duplicato?)\n");
                    report_destroy(new_report); 
                }
                break;
            }
            
            case 0:
                printf("\nChiusura del sistema...\n");
                break;

            default:
                printf("\nScelta non valida.\n");
        }
        
        if (choice != 0) {
            printf("\nPremi INVIO per tornare al menu' principale...");
            clear_input_buffer();
            getchar();
            system("cls || clear");
        }
        
    } while (choice != 0);

    gestionale_destroy(g);
    return 0;
}