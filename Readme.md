# Progetto Gestionale Report

### IMPORTANTE: Directory di Lavoro
Per garantire il corretto caricamento del database (db.txt) e dei file di test, il software deve essere eseguito posizionandosi nella cartella **build** del progetto.

**Esecuzione del Gestionale:**
```bash
./gestione_report.exe
```

**Esecuzione dei Test:**
```bash
./run_tests.exe
```

### Compilazione
- Digitare `make` nella radice per compilare tutto.

---

### Struttura
- **/include**: File header (.h).
- **/src**: Implementazione delle ADT (.c).
- **/tests**: Test suite e casi di test (Input/Oracle).
- **/build**: Contiene gli eseguibili generati.
- **db.txt**: File di persistenza dei dati (posizionato nella radice).

---

### Memory management
- Ogni ADT è responsabile della deallocazione delle proprie risorse.
- L'ADT Gestionale coordina la distruzione atomica di liste e code per prevenire memory leak.

---

### Informazioni importanti
- Non toccare db.txt
- Bisogna salvare le modifiche effettuate prima di uscire dal programma via l'opzione "Salva ed esci"
- Gli id dei report vengono generati automaticamente, l'utente non deve toccare il file db.txt