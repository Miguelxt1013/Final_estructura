#include <iostream>
#include <cstdlib>
#include <iomanip>
using namespace std;

struct nodo {
    int valor;
    nodo *izq;
    nodo *der;
    int altura;
};

struct nodo *raiz = NULL, *aux = NULL, *aux2 = NULL, *padre = NULL;

int obtenerAltura(nodo *n) {
    return (n == NULL) ? 0 : n->altura;
}

void actualizarAltura(nodo *n) {
    n->altura = 1 + max(obtenerAltura(n->izq), obtenerAltura(n->der));
}

int obtenerFactorBalanceo(nodo *n) {
    return (n == NULL) ? 0 : obtenerAltura(n->izq) - obtenerAltura(n->der);
}

nodo* rotacionDerecha(nodo *y) {
    nodo *x = y->izq;
    nodo *T2 = x->der;

    x->der = y;
    y->izq = T2;

    actualizarAltura(y);
    actualizarAltura(x);

    return x;
}

nodo* rotacionIzquierda(nodo *x) {
    nodo *y = x->der;
    nodo *T2 = y->izq;

    y->izq = x;
    x->der = T2;

    actualizarAltura(x);
    actualizarAltura(y);

    return y;
}

nodo* balancear(nodo *n) {
    actualizarAltura(n);
    int balance = obtenerFactorBalanceo(n);

    if (balance > 1 && obtenerFactorBalanceo(n->izq) >= 0)
        return rotacionDerecha(n);

    if (balance > 1 && obtenerFactorBalanceo(n->izq) < 0) {
        n->izq = rotacionIzquierda(n->izq);
        return rotacionDerecha(n);
    }

    if (balance < -1 && obtenerFactorBalanceo(n->der) <= 0)
        return rotacionIzquierda(n);

    if (balance < -1 && obtenerFactorBalanceo(n->der) > 0) {
        n->der = rotacionDerecha(n->der);
        return rotacionIzquierda(n);
    }

    return n;
}

nodo* insertarAVL(nodo *raiz, int valor) {
    if (raiz == NULL) {
        nodo *nuevo = (nodo *)malloc(sizeof(nodo));
        nuevo->valor = valor;
        nuevo->izq = nuevo->der = NULL;
        nuevo->altura = 1;
        return nuevo;
    }

    if (valor < raiz->valor)
        raiz->izq = insertarAVL(raiz->izq, valor);
    else if (valor > raiz->valor)
        raiz->der = insertarAVL(raiz->der, valor);
    else
        return raiz;

    return balancear(raiz);
}

int registrar() {
    int valor;
    cout << "Digite el valor del nodo: ";
    cin >> valor;
    raiz = insertarAVL(raiz, valor);
    return 0;
}

nodo* nodoMinimo(nodo *n) {
    nodo *actual = n;
    while (actual->izq != NULL)
        actual = actual->izq;
    return actual;
}

nodo* eliminarAVL(nodo *raiz, int valor) {
    if (raiz == NULL) return raiz;

    if (valor < raiz->valor)
        raiz->izq = eliminarAVL(raiz->izq, valor);
    else if (valor > raiz->valor)
        raiz->der = eliminarAVL(raiz->der, valor);
    else {
        if ((raiz->izq == NULL) || (raiz->der == NULL)) {
            nodo *temp = raiz->izq ? raiz->izq : raiz->der;

            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else
                *raiz = *temp;

            free(temp);
        } else {
            nodo *temp = nodoMinimo(raiz->der);
            raiz->valor = temp->valor;
            raiz->der = eliminarAVL(raiz->der, temp->valor);
        }
    }

    if (raiz == NULL) return raiz;

    return balancear(raiz);
}

int eliminar() {
    int valor;
    cout << "Digite el valor a eliminar: ";
    cin >> valor;
    raiz = eliminarAVL(raiz, valor);
    return 0;
}

void imprimirJerarquico(nodo *n, int espacio = 0, int incremento = 5) {
    if (n == NULL) return;

    espacio += incremento;
    imprimirJerarquico(n->der, espacio);

    cout << endl;
    cout << setw(espacio) << n->valor;

    imprimirJerarquico(n->izq, espacio);
}

int mostrar() {
    if (raiz) {
        cout << "\nArbol AVL en formato jerarquico:" << endl;
        imprimirJerarquico(raiz);
        cout << endl;
    } else {
        cout << "El arbol esta vacio." << endl;
    }
    return 0;
}

int main() {
    int opc;
    do {
        cout<< "------------ Menu -------------"<<endl;
        cout << "1. Registrar" << endl;
        cout << "2. Mostrar" << endl;
        cout << "3. Eliminar" << endl;
        cout << "4. Salir" << endl;
        cout << "Ingrese una opcion: ";
        cin >> opc;
        switch (opc) {
            case 1:
                registrar();
                break;
            case 2:
                mostrar();
                break;
            case 3:
                eliminar();
                break;
            case 4:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }
    } while (opc != 4);
    return 0;
}
