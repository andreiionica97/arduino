# arduino
MagneBall - jocul pare putin greu la inceput, dar dupa ce este invatata miscarea bilei acesta devine floare la ureche.

Componente utilizate:
- Arduino Uno
- Cablu Serial
- Matrice de leduri 8x8
- Driver MAX7219
- LCD display
- Push button
- Condensator 10 uF, 100 nF
- Rezistente 100K, 220 Ohm
- Breadboard-uri
- Fire de conexiune
- Cutie carton
- Banda adeziva

Despre: Scopul jocului este obtinerea unui punctaj cat mai mare. Scorul creste colectand punctele bonus sau doar prin simpla deplasare a bilei. Pentru deplasare scorul primit este calculat in functie de dificultatea nivelului silectat. Jocul se termina atunci cand bila atinge unul din cei doi electromagneti (cele 2 linii aprinse), sau unul dintre cele 2 obstacole care se gasesc pe prima si ultima coloana.

Recomandare: Folosirea degetului mare pentru a apasa butoanele.

Reguli: Pentru a incepe jocul se apasa butonul din dreapta. Urmeaza selectarea dificultatii. Prin apasarea butonului din dreapta putem alege intre 4 nivele de dificulate (0 - cel mai usor, nu contine obstacole - recomandat incepatorilor; 3 - cel mai greu). Punctul de plecare este in coltul din stanga sus. In mod natural electromagnetul de jos este pornit, iar la fiecare miscare a bilei aceasta coboara spre el. La apasarea butonului din dreapta se porneste electromagnetul de sus iar cel de jos se opreste (procesul se inverseaza la eliberarea butonului). Secretul mentinerii unei pozitii favorabile este apasarea butonului pentru o perioada de cateva milisecunde pentru a urca mai multe pozitii. La intalnirea cu peretii din stanga sau dreapta bila isi schimba directia de mers. Obstacolele de pe verticala se genereaza random cand bila este in partea opusa. In timul jocului pe lcd sunt afisate si actualizate in timp real scorul, dificultatea si viteza cu care se misca bila. Dupa ce bila atinge un electromagnet sau un obstacol apare pentru cateva secunde mesajul Game Over, dupa care apare un mic meniu din care se poate selecta vizualizarea scorului (butonul din stanga), sau repornirea jocului (butonul din dreapta). In cazul optarii pentru vizualizarea scorului dupa cateva secunde in care acesta impreuna cu scorul record sunt afisate se revine la meniul de mai inainte.
  
Disclaimer: Uneori lcd-ul afiseaza caractere incorecte. In acest caz jocul trebuie repornit pentru a putea salva si afisa scorul.
