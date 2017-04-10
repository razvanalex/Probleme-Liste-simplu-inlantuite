#include<stdio.h>
#include<stdlib.h>

typedef int TEL;

typedef struct celula
{
    TEL info;
    struct celula *urm;
} TCelula, *TLista, **ALista;

/*
    Exercitiul 1:
    Sa se creeze o lista simplu inlantuita cu elemente intregi prin:
    a) inserare la inceput
    b) inserare la final
    c) inserare ordonata
    
    Exercitiul 2:
    Sa se insereze:
    a) dupa un element de referinta
    b) inaintea unui element de referinta
    
    Exercitiul 3:
    Sa se afiseze lista rezultata
    
    Exercitiul 4:
    Sa se caute un element si sa se returneze un pointer la el
    
    Exercitiul 5:
    Sa se elimine un element
    
    Exercitiul 6:
    Sa se copieze o lista dupa un criteriu 
    (pointer la functie):
    a) elemente pare
    b) in ordiea inversa
    c) lista rezultat este ordonata crescator
    
    Exercitiul 7:
    Sa se mute elementele dintr-o lista dupa un criteriu 
    (pointer la functie)
    
    Exercitiul 8
    Sa se elimine duplicatele consecutive
    
    Exercitiul 9 (BONUS)
    Sa se ordoneze o lista de intregi
    
    Exercitiul 10:
    Sa se distruga lista (eliberare de memorie)
    
    Nota: Se va testa cu valgrind pentru a verifica daca nu exista erori sau
    memory leaks

*/

// -----------------
//  REZOLVARI:
// -----------------

/* Functii pregatitoare */
TLista AlocCelula(TEL e)
{
    TLista aux;
    
    aux = (TLista)malloc(sizeof(TCelula));
    
    // verificare alocare
    if (!aux)
        return NULL;
    
    aux->info = e;
    aux->urm = NULL;
    
    return aux;
}

/* Exercitiul 1 */
// a)
int InsInc(TLista *l, TEL e)
/*
    Returneaza:
        0 - eroare de alocare
        1 - succes
*/
{
    TLista aux = NULL;
    
    aux = AlocCelula(e);
    if (!aux)
        return 0;
    
    aux->urm = *l;
    *l = aux;

    return 1;
}
// b)
int InsSf(TLista *L, TEL el)
{
    TLista aux, ultim;
    
    // pozitionare la final
    for (; *L != NULL; ultim = *L, L = &(*L)->urm);
    
    // creare celula + verificare
    aux = AlocCelula(el);
    if (!aux)
        return 0;
    
    // adaugare in lista
    if (ultim)
        ultim->urm = aux;
    else 
        *L = aux;
    
    return 1;
}
// c)
int cmpInt(int a, int b)
{
    return b - a;
}
int InsOrd(ALista L, TEL el, int(*cmp)(int, int))
{
    TLista aux;
    
    for (; *L != NULL; L = &(*L)->urm)
        if (cmp((*L)->info, el) < 0)
            break;

    aux = AlocCelula(el);
    if (!aux)
        return 0;
        
    aux->urm = *L;
    *L = aux;
    
    return 1;
}

/* Exercitiul 2 */
// a)
int InsDupaRef(TLista L, TEL el, TEL ref)
{
    TLista aux;
    
    for(; L != NULL; L = L->urm)
        if (L->info == ref)
            break;
    
    if(!L)
        return -1;
    
    aux = AlocCelula(el);
    if (!aux)
        return 0;
    
    aux->urm = L->urm;
    L->urm = aux;
   
    return 1;
}
// b)
int InsInainteRef(ALista L, TEL el, TEL ref)
{
    TLista aux;
    
    for (; *L != NULL; L = &(*L)->urm)
        if ((*L)->info == ref)
            break;
            
    if (*L == NULL)
        return -1;
    
    aux = AlocCelula(el);
    if (!aux)
        return 0;
        
    aux->urm = *L;
    *L = aux;
    
    return 1;
}

/* Exercitiul 3 */
void AfisareLista(TLista l)
{
    printf("Lista: ");
    for (; l != NULL; l = l->urm)
        printf("%d ", l->info);
    printf("\n");
}

/* Exercitiul 4 */
TLista Cauta(TLista l, TEL el)
{
    for (; l != NULL; l = l->urm)
        if (l->info == el)
            return l;
            
    return NULL;
}

/* Exercitiul 5 */
int ElimEl(TLista *l, TEL el)
{
    TLista p = *l, ant = NULL;
    for (; p != NULL; ant = p, p = p->urm)
        if (p->info == el)
            break;
    
    if (!p)
        return -1;
    
    if (ant)
        ant->urm = p->urm;
    else
        *l = p->urm;
    
    free(p);
    return 1;
}

/* Exercitiul 6 */
// a)
TLista CopiereElPare(TLista l)
{
    TLista r = NULL, aux, ultim = NULL;
    
    for (; l != NULL; l = l->urm)
    {
        if (l->info % 2 == 0)
        {
            aux = AlocCelula(l->info);
            if (!aux)
                break;
            
            if (ultim)
            {    
                ultim->urm = aux;
                ultim = ultim->urm;
            }
            else 
            {
                r = aux;
                ultim = r;
            }
            
        }
    }
    
    return r;
}
// b)
TLista CopiereInversa(TLista l)
{
    TLista r = NULL, aux;
    
    for(; l != NULL; l = l->urm)
    {
        aux = AlocCelula(l->info);
        if (!aux)
            return r;
            
        if (r)
            aux->urm = r;
        r = aux;
    }
    
    return r;
}
// c)
TLista CopiereOrdonat(TLista l)
{
    TLista r = NULL;
    
    for (; l != NULL; l = l->urm)
        InsOrd(&r, l->info, cmpInt);
    
    return r;
}

/* Exercitiul 7 */
int nrPar(int a)
{
    if (a % 2 == 0)
        return 1;
    return 0;
}
TLista Muta(TLista *l, int(*crit)(int))
/*
    crit returneaza:
        1 daca e adevarat
        0 daca e fals
*/
{
    TLista p = *l, ant = NULL, r = NULL, ultim;
    
    while(p != NULL)
    {
        if (crit(p->info))
        {
            if (ant)
                ant->urm = p->urm;
            else 
                *l = p->urm;
            
            if (!r)
            {
                 r = p;
                 ultim = r;
            }
            else
            {
                ultim->urm = p;
                ultim = ultim->urm;
            }
            ultim->urm = NULL;
            
            if (!ant)
                p = *l;
            else 
                p = ant->urm;
        }
        else 
        {
            ant = p;
            p = p->urm;
        }
    }
    
    return r;
}

/* Exercitiul 8 */
int ElimDupl(TLista l)
{
    TLista u;
    int i = 0;
    
    if (!l)
        return 0;
   
    while(l->urm != NULL)
    {
        u = l->urm;
        if (l->info == u->info)
        {
            l->urm = u->urm;
            free(u);
            i++;
        }
        else l = l->urm;
    }
    return i;
}

/* Exercitiul 9 */
void Ordonare(TLista *l)
{
    TLista p = *l, r = NULL, rant, rp;
    while (p != NULL)
    {
        rant = NULL;
        for (rp = r; rp != NULL; rant = rp, rp = rp->urm)
            if(rp->info > p->info)
                break;
        
        if(!rant)
            r = p;
        else rant->urm = p;
        
        *l = p->urm;
        if(!r)
            p->urm = NULL;
        else p->urm = rp;
         
        p = *l;
    }
    
    *l = r;
}

/* Exercitiul 10 */
void DistrLista(TLista *l)
{
    if (!l)
        return;
        
    TLista p = *l;    
    while(p)
    {
       *l = p->urm;
       free(p);
       p = *l;
    }
       
    *l = NULL;
}

int main()
{
    int ex, x, ref;
    TLista l1 = NULL, l2 = NULL;
    char ch;
    
    do {
        printf("Pentru a iesi alege 0 ca exercitiu.\n");
        printf("Alege exercitiul (1-9): ");
        scanf("%d", &ex);
        
        switch (ex)
        {
            case 0: break;
            case 1: // Exercitiul 1
                printf("Alege subpunctul(a-c): ");
                scanf("%c", &ch);
                while((ch = getchar()) && (ch == '\n' || ch == EOF));
                switch(ch)
                {
                    case 'a': // Punctul a)
                        printf("Introducere elemente: ");
                        while (scanf("%d", &x))
                            if (InsInc(&l1, x) == 0)
                                printf("Inserare esuata!\n");
                        while((ch = getchar()) && (ch == '\n' || ch == EOF));
                        break;
                        
                    case 'b': // Punctul b)
                        printf("Introducere elemente: ");
                        while (scanf("%d", &x))
                            if (InsSf(&l1, x) == 0)
                                printf("Inserare esuata!\n");
                        while((ch = getchar()) && (ch == '\n' || ch == EOF));
                        break;
                    
                    case 'c': // Punctul c)
                        if (!l1)
                        {
                            printf("Introducere elemente: ");
                            while (scanf("%d", &x))
                            if (InsOrd(&l1, x, cmpInt) == 0)
                                printf("Inserare esuata!\n");
                            while((ch = getchar()) && (ch == '\n' || ch == EOF));
                        }
                        else printf("Lista nevida!!\n");
                        break;
                    
                    default: printf("Alegere invalida!"); break;
                }
                break;
                
            case 2:
                printf("Alege subpunctul(a-b): ");
                scanf("%c", &ch);
                while((ch = getchar()) && (ch == '\n' || ch == EOF));
                switch(ch)
                {
                    case 'a': // Punctul a)
                        printf("Introducere element: ");
                        scanf("%d", &x);
                        printf("Introduce valoarea de referinta: ");
                        scanf("%d", &ref);
                        if (InsDupaRef(l1, x, ref) != 1)
                            printf("Inserare esuata!\n");
                        break;
                        
                    case 'b': // Punctul b)
                        printf("Introducere element: ");
                        scanf("%d", &x);
                        printf("Introduce valoarea de referinta: ");
                        scanf("%d", &ref);
                        if (InsInainteRef(&l1, x, ref) != 1)
                            printf("Inserare esuata!\n");
                        break;
                    
                    default: printf("Alegere invalida!"); break;
                }
                
                break;
                
            case 3:
                AfisareLista(l1);
                break;
                
            case 4:
                printf("Introducere element: ");
                scanf("%d", &x);
                TLista lAux = Cauta(l1, x);
                if (lAux)
                {
                    printf("Adresa elementului cautat este %p si ", lAux);
                    if (lAux->urm)
                        printf("are ca element urmator %d.\n", lAux->urm->info);
                    else printf("nu are element urmator.\n");
                }
                else printf("Elementul nu a fost gasit.\n");
                break;
                
            case 5:
                printf("Introducere element: ");
                scanf("%d", &x);
                if (ElimEl(&l1, x) == -1)
                    printf("Eliminare esuata\n");
                break;
                
            case 6:
                printf("Alege subpunctul(a-c): ");
                scanf("%c", &ch);
                while((ch = getchar()) && (ch == '\n' || ch == EOF));
                switch(ch)
                {
                    case 'a': // Punctul a)
                        l2 = CopiereElPare(l1);
                        AfisareLista(l2);
                        DistrLista(&l2);
                        break;
                    case 'b': // Punctul b)
                        l2 = CopiereInversa(l1);
                        AfisareLista(l2);
                        DistrLista(&l2);
                        break;
                    case 'c': // Punctul c)
                        l2 = CopiereOrdonat(l1);
                        AfisareLista(l2);
                        DistrLista(&l2);
                        break;
                    default: printf("Alegere invalida!"); break;
                }
                break;
                
            case 7:
                l2 = Muta(&l1, nrPar);
                AfisareLista(l2);
                DistrLista(&l2);
                break;
                
            case 8:
                printf("Au fost eliminate %d elemente.", ElimDupl(l1));
                break;
                
            case 9:
                Ordonare(&l1);
                break;
            default: printf("Alegere invalida!\n");
        }
        printf("\n\n");
    } while(ex != 0);
    
    DistrLista(&l1);
    
    return 0;
}