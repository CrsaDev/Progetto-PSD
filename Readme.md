IMPORTANTE: Directory di Lavoro
Per garantire il corretto caricamento del database (db.txt) e dei file di test, il software deve essere eseguito posizionandosi nella cartella build del progetto.

Esecuzione del Gestionale:
./gestione_report.exe

Esecuzione dei Test:
./run_tests.exe

Compilazione:
Digitare make nella radice per compilare tutto.

Struttura:
/include: File header (.h)
/src: Implementazione delle ADT (.c).
/tests: Test suite e casi di test (Input/Oracle).
/build: Contiene gli eseguibili generati.
db.txt: File di persistenza dei dati (posizionato nella radice).

Memory management:
Ogni ADT è responsabile della deallocazione delle proprie risorse.
L'ADT Gestionale coordina la distruzione atomica di liste e code per prevenire memory leak.