#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

#define RangoDeSalto 10
#define VACIA -1

using namespace std;
#define log(x) cout << x << endl

//                      DEFINICION DE ITERADORES Y ESTRUCTRAS DE DATOS

int N, M; // numero de productos y ofertas a leer de los archivos .dat, respectivamente


struct producto {   
    int cod_producto;
    char nombre_producto[31];
    int precio;
};

struct oferta {
    int cod_producto;
    int cantidad_descuento;
    int descuento;
    int productos_equivalentes[10];
};

struct TipoProductoInfo{
    int ID_Grupo; // grupos_equivalentes
    int precio;
};

struct TipoOfertaInfo{
    int cantidad_descuento;
    int descuento;
    int productos_equivalentes[10];
};

struct ranuraProducto {
    int clave;
    TipoProductoInfo info;
};

struct ranuraOferta {
    int clave;
    TipoOfertaInfo info;
};

struct Grupo {
    int arreglo[11];
};

struct tNodo{
    Grupo info;
    tNodo* sig;
};


// Es una lista dinamica que almacena structs del tipo "Grupo", los cuales contienen un arreglo de 11 elementos que serán un grupo de ofertas
class tListaArreglo {
    private:
        tNodo* head;
        tNodo* tail;
        tNodo* curr;
        int listSize;
        int pos;
    
    public:


/*****
* Constructor tListaArreglo
******
* resumen: Crea 2 nodos(head y tail) y un nodo "curr", donde head sera el primer nodo y tail sera el ultimo y tendra un puntero hacia un nullptr
******
* Input: No recibe.
* tipoParámetro NombreParámetro : No recibe.
* .......
******
* Returns: no retorna.
* TipoRetorno, Descripción retorno: no retorna.
*****/
    tListaArreglo(){
        head = new tNodo;
        tail = new tNodo;
        curr = head;
        head->sig = tail;
        tail->sig = nullptr;

        listSize = 0; 
        pos = 0;
        

    };

/*****
* TipoFunción NombreFunción: Destructor tListaArreglo
******
* Resumen Función: Se mueve hacia el primer elemento de la lista y la recorre completa eliminando todos sus nodos.
******
* Input: No recibe.
* tipoParámetro NombreParámetro : Descripción Parámetro: No recibe.
* .......
******
* Returns: No retorna
* TipoRetorno, Descripción retorno: No retorna
*****/
    ~tListaArreglo(){
        int i = 0;
        while(i < listSize){
            tNodo *actual = head;
            head = actual->sig;
            delete actual;
            i++;
        };
        log("Lista borrada correctamente");
    };

/*****
* TipoFunción NombreFunción: int, getListaSize.
******
* Resumen Función: retorna el tamaño de la lista.
******
* Input: no recibe.
* tipoParámetro NombreParámetro : Descripción Parámetro: No recibe.
* .......
******
* Returns: retorna el tamaño de la lista.
* TipoRetorno, Descripción retorno: int, retorna el tamaño de la lista.
*****/
    int getListSize(){
        return listSize;
    };


/*****
* TipoFunción NombreFunción: int, getPos
******
* Resumen Función: retorna la posicion del nodo curr
******
* Input:
* tipoParámetro NombreParámetro : Descripción Parámetro: no recibe
* .......
******
* Returns:
* TipoRetorno, Descripción retorno: int
*****/
    int getPos(){
        return pos;
    };
/*****
* TipoFunción NombreFunción: void, insertElem
******
* Resumen Función: Recibe un un elemento y una posicion para insertar en el arreglo de la posicion del nodo curr
******
* Input: dos int's
* tipoParámetro NombreParámetro : Descripción Parámetro: dos int's, el item y la posicion dentro del arreglo respectivamente
* .......
******
* Returns: no retorna
*****/
    void insertElem(int item, int index){
        curr->info.arreglo[index] = item;
    };


/*****
* Resumen Función: Crea un nodo y llena el arreglo correspondiente a su struct grupo con -1 en todas sus posiciones
******
* Input:
* tipoParámetro NombreParámetro : Descripción Parámetro:
* .......
******
* Returns:
* TipoRetorno, Descripción retorno:
*****/
    void nuevoGrupo(){
        if(head->sig == tail) {
            curr = new tNodo;
            int* pointer = curr->info.arreglo;
            pointer[0] = 99;
            head->sig = curr;
            curr->sig = tail;

            listSize++;
            pos++;
        }
        else {
            curr->sig = new tNodo;
            curr = curr->sig;
            curr->sig = tail;

            listSize++;
            pos++;
        };

        for(int i = 0; i < 11; i++) curr->info.arreglo[i] = VACIA;
    };

/*****
* Resumen Función: retorna el arreglo correspondiente a curr
*****/
    int* getArreglo(){
        return curr->info.arreglo;
    };


/*****
* Resumen Función: Mueve el puntero curr a la primera posicion de la lista
*****/
    void moveToStart(){
        curr = head->sig;
        pos = 1; 
    };

/*****
* Resumen Función: mueve el puntero curr a la siguiente posicion
*****/
    void next(){ 
        if (curr != tail){
            curr = curr->sig; 
            pos++;
        };
    };

/*****
* Resumen Función: sirve para visualizar todos los arreglos dentro de la lista. se mueve a la primera posicion e itera a lo largo de toda la lista
*****/
    void showList(){

        curr = head->sig;
        pos = 1;

        for (int i = 0; i < listSize; i++){
        int* ola = curr->info.arreglo;
        log("Lista x:");
        for (int u = 0; u < 11; u++){
            log(ola[u]);
        };
        curr = curr->sig;
        pos++;
        };
    };
};

//                      ----------------------------
//                      DEFINICION DE FUNCIONES HASH 
//                      ----------------------------
// en esencia todas las funciones de un mismo tipo son la misma, pero adaptadas a los datos de cada hash
int Primo_Relativo_Menor(int size){
    /*
    * Resumen Función: busca el numero primo menor mas proximo al size 
    **
    * Input: int size : numero a buscar el primo mas cercano
    **
    * Returns: int max : numro con el primo mas proximo a size
    */
    int size_arreglo = size;
    if(size_arreglo == 0 || size_arreglo == 1) return 3;
    bool primos[size_arreglo];

    for(int a = 0; a < size_arreglo; a++) {
        primos[a]= true; //inicializa todos los numeros en true
    };
    for(int b = 2; b <= size_arreglo; b++){
        int k = 2;
        while(b*k < size_arreglo){
            primos[b*k] = false; // todos los multiplos de b son colocados en false
            k++;
        };
    };

    int max;
    for(int c = 0; c < size_arreglo; c++){
        if(primos[c] == true){
            max = c;
        };
    };
    return max;
}

// funcion hash, localiza valores de acuerdo a su clave 
int h1(int k, int Size){
    /*
    * Resumen Función: busca el valor de la posicion de la clave k en el hash
    **
    * Input: int k : clave a la cual se le busca la informacion
    *        int Size : es el tamaño del hash
    **
    * Returns: int pos : la posicion en el hash en la que se encuentra la informacion de la clase k
    */
    int pos = k % Size;
    return pos;
};

int h2(int k, int Size){
    /*
    * Resumen Función: calcula en tamaño del salto en el hash para la posterior solucion de coliciones
    **
    * Input:  int k: clave a la cual se le busca la informacion
    *         int Size: tamaño del hash
    **
    * Returns: int salto : el tamaño del salto a realizar en la insersion/busqueda de elementos en el hash
    */
    int salto = k % Primo_Relativo_Menor(Size);

    return salto; 
};

int p(int k, int pos, int size){
    /*
    * TipoFunción NombreFunción: int p : politica de solucion de colisiones
    **
    * Resumen Función: soluciona los problemas de coliciones aplicando un dodle hashing
    **
    * Input: int k : clave a la cual se le busca la informacion
    *        int pos: posicion donde ocurre la colicion
    *        int size : tamaño del hash
    **
    * Returns: int temp : la posicion donde se podra insertar el elemento en el hash
    */
    int temp = pos*h2(k, size);
    return temp;
};

//                      FUNCIONES DEL HASH
// FUNCIONES INSERT
bool InsertHashProductos(ranuraProducto* HashP, producto& producto, int SizeP){
    /*
    * TipoFunción NombreFunción: bool InsertHashProductos
    **
    * Resumen Función: Inserta los elementos en el hash de los Productos, y retorna un booleano con el resultado de la insercion
    **
    * Input: HashP : son las ranuras del hash de los productos
    *        producto: es el struct con la informacion del producto a insertar
    *        int SizeP : tamaño del hash
    **
    * Returns: bool = 0 corresponde a que el elemento no se pudo insertar en el hash
    *          bool = 1 corresponde a que el elemento se pudo insertar se forma exitosa en el hash
    */
    int k = producto.cod_producto;
    int inicio;
    int pos = inicio = h1(k, SizeP);
    int colisiones = 0;
    for (int i = 1; HashP[pos].clave != VACIA && HashP[pos].clave != k; i++){
        pos = (inicio + p(k,i,SizeP)) % SizeP; // proxima ranura de la secuencia
        colisiones++;
    };

    if (HashP[pos].clave == k)
        return 0; // elemento no se pudo insertar
    else {
        HashP[pos].clave = k;
        HashP[pos].info.precio = producto.precio;
        return 1; // elemento insertado correctamente
    };
};

bool InsertHashOfertas(ranuraOferta* HashO, oferta& oferta, int SizeO){
    /*
    * TipoFunción NombreFunción: bool InsertHashOfertas
    **
    * Resumen Función: Inserta los elementos en el hash de las Ofertas, y retorna un booleano con el resultado de la insercion
    **
    * Input: HashO : son las ranuras del hash de los productos
    *        oferta : es el struct con la informacion de la oferta a insertar
    *        int SizeO : tamaño del hash
    **
    * Returns: bool = 0 corresponde a que el elemento oferta no se pudo insertar en el hash
    *          bool = 1 corresponde a que el elemento oferta se pudo insertar se forma exitosa en el hash
    */
    int k = oferta.cod_producto;
    
    int inicio;
    int pos = inicio = h1(k, SizeO);
    int colisiones = 0;

    for(int i = 1; HashO[pos].clave != VACIA && HashO[pos].clave != k; i++){
        pos = (inicio + p(k,i,SizeO)) % SizeO; // proxima ranura de la secuencia
        colisiones++;
    }; 
    if (HashO[pos].clave == k) 
        return 0; // elemento no se pudo insertar. ya se encuentra en el hash 
    else {

        HashO[pos].clave = k;
        HashO[pos].info.cantidad_descuento = oferta.cantidad_descuento;
        HashO[pos].info.descuento = oferta.descuento;
        for(int c = 0; c < 10; c++) HashO[pos].info.productos_equivalentes[c] = oferta.productos_equivalentes[c];
        return 1; // elemento insertado correctamente
    };
};

// FUNCIONES GET VALUE / INFO

/*****
* Resumen Función: Retorna el struct asociado al producto que se reciba como parametro "int cod"
******
* Input: HasP: un arreglo de structs, el cual representa al hash de productos; cod: Codigo del producto; SizeP: Tamaño del hashP
* tipoParámetro NombreParámetro : Descripción Parámetro:
* .......
******
* Returns: Retorna un struct "TipoProductoInfo"
*****/
TipoProductoInfo HashP_getInfo(ranuraProducto* HashP, int cod, int SizeP){
    int inicio;
    int pos = inicio = h1(cod, SizeP);
    int colisiones = 0;

    for(int i = 1; HashP[pos].clave != VACIA && HashP[pos].clave != cod; i++){
        pos = (inicio + p(cod,i,SizeP)) % SizeP; // proxima ranura de la secuencia
        colisiones++;
    }; 
    if (HashP[pos].clave == cod){

        return HashP[pos].info; // elemento insertado correctamente
    } 
    else {
        TipoProductoInfo P;
        P.precio = 12345678; // Valor arbitrario
        return P;
    };
};

/*****
* Resumen Función: Retorna el struct asociado a la oferta del producto que se reciba como parametro "int cod"
******
* Input: HasO: un arreglo de structs, el cual representa al hash de ofertas; cod: Codigo del producto; SizeO: Tamaño del hashO
* tipoParámetro NombreParámetro : Descripción Parámetro:
* .......
******
* Returns: Retorna un struct "TipoOfertaInfo"
*****/
TipoOfertaInfo HashO_getInfo(ranuraOferta* HashO, int cod, int SizeO){
    int inicio;
    int pos = inicio = h1(cod, SizeO);
    int colisiones = 0;

    for(int i = 1; HashO[pos].clave != VACIA && HashO[pos].clave != cod; i++){
        pos = (inicio + p(cod,i,SizeO)) % SizeO; // proxima ranura de la secuencia
        colisiones++;
    }; 
    if (HashO[pos].clave == cod){

        return HashO[pos].info; // elemento insertado correctamente
    } 
    else {
        TipoOfertaInfo O;
        O.descuento = 12345678;
        return O;
    };
};


/*****
* Resumen Función: retorna la "llave" para acceder a la informacion de un producto en el hash
******
* Input: HasP: un arreglo de structs, el cual representa al hash de productos; cod: Codigo del producto; SizeP: Tamaño del hashP
* tipoParámetro NombreParámetro : Descripción Parámetro:
* .......
******
* Returns: Retorna un int el cual corresponde a la "llave" del producto "cod"
*****/
int HashP_getKey(ranuraProducto* HashP, int cod, int SizeP){
    int inicio;
    int pos = inicio = h1(cod, SizeP);
    int colisiones = 0;

    for(int i = 1; HashP[pos].clave != VACIA && HashP[pos].clave != cod; i++){
        pos = (inicio + p(cod,i,SizeP)) % SizeP; // proxima ranura de la secuencia
        colisiones++;
    }; 
    if (HashP[pos].clave == cod){

        return pos;
    } 
    else {
        return VACIA;
    };
};




/*****
* Resumen Función: Funcion booleana que retorna true si un producto tiene oferta, esto lo hace buscando dicho producto en el hash ofertas.
******
* Input: HasO: un arreglo de structs, el cual representa al hash de productos; cod_producto: Codigo del producto; SizeO: Tamaño del hashO
* tipoParámetro NombreParámetro : Descripción Parámetro:
* .......
******
* Returns: Retorna un Booleano true si encuentra al producto y false en el caso contrario
*****/
bool tieneOferta(ranuraOferta* HashO, int cod_producto, int SizeO){
    int k = cod_producto;
    int inicio;
    int pos = inicio = h1(k, SizeO);
    int colisiones = 0;
    for (int i = 1; HashO[pos].clave != VACIA && HashO[pos].clave != k; i++){
        pos = (inicio + p(k,i,SizeO)) % SizeO; // proxima ranura de la secuencia
        colisiones++;
    }; 
    if (HashO[pos].clave == k){
        return 1; // elemento encontrado 
    } 
    else {
        return 0; // no se encuentra en el hash 
    };
};

// calculamos el maximo comun divisor entre dos numeros
// si es 1 = son coprimos, retornamos 1
// si es distinto de 1, retornamos 0
bool CoPrimos(int a, int b){ 
    for(int k = 2; k < b; k++){         // b es el menor numero
    };
    return 1;
};


/*****
* Resumen Función: Retorna un int asociado al tamaño del hash para que este siempre tenga una carga de 0.7
******
* Input: n_Entradas: Cantidad de productos u ofertas que seran insertadas dentro del hash
* tipoParámetro NombreParámetro : Descripción Parámetro:
* .......
******
* Returns: Retorna un int que corresponde al tamaño del hash
*****/
int RecomendedSizeHash(int n_Entradas){ // para un factor de carga aprox de 0.7
    int sizeRecom_entradas = round(n_Entradas*(1.6));

    if(sizeRecom_entradas == 0) return 1;  // dificil o imposible que pase

    //int size_final = Primo_Relativo_Menor(sizeRecom_entradas);
    return sizeRecom_entradas;
};

/*****
* Resumen Función: Busca en el arreglo de la lista enlazada un producto, si lo encuentra retorna True y en caso contrario False
******
* Input: codigo: El codigo del producto a buscar; Grupo_a_Revisar: puntero al arreglo donde se quiere encontrar el codigo.
* tipoParámetro NombreParámetro : Descripción Parámetro:
* .......
******
* Returns: Retorna un struct "TipoProductoInfo"
*****/
bool CodigoEnGrupo(int codigo, int* Grupo_a_Revisar){
    for(int z = 0; z < 11; z++){
        if(codigo == Grupo_a_Revisar[z]) return 1;
    };    
    return 0;      
};

int main(){

    //                      -------------------
    //                      LECTURA DE ARCHIVOS + inicializacion de HashProductos y HashOfertas
    //                      -------------------

    fstream fp;

    // LECTURA ARCHIVO PRODUCTOS + CREACION HASH PRODUCTOS

    fp.open("productos.dat", ios::in | ios::binary);
    fp.read((char *) &N, sizeof(int));
    int sizeP = RecomendedSizeHash(N);                      // Tamaño eficiente del hash (en funcion del numero de productos) 
    int productos[N];                                       // lista con todos los codigos de todos los productos 
    ranuraProducto HashProductos[sizeP];                    // Hash de Productos inicializado 

    log("{------ Se está generando su boleta, por favor espere ------}");
                               
    for(int j = 0; j < sizeP; j++){
        HashProductos[j].clave = VACIA;                     // todas las claves quedan como VACIAS
        HashProductos[j].info.precio = 0;
        //HashProductos[j].info.ID_Grupo = VACIA;
    };
     
    producto p1;        // Se ingresan todos los productos (que estan dentro del arr productos) en el hash
    for(int j = 0; j < N; j++){
        fp.read((char *) &p1, sizeof(producto));
        productos[j] = p1.cod_producto;
        InsertHashProductos(HashProductos, p1, sizeP);      
    };
    fp.close();

    // LECTURA ARCHIVO OFERTAS + CREACION HASH OFERTAS

    fp.open("ofertas.dat", ios::in | ios::binary);
    fp.read((char *) &M, sizeof(int));
    int sizeO = RecomendedSizeHash(M);
    ranuraOferta HashOfertas[sizeO];

    for(int j = 0; j < sizeO; j++){
        HashOfertas[j].clave = VACIA;
        for(int m = 0; m < 10; m++) HashOfertas[j].info.productos_equivalentes[m] = VACIA;
    };  

    oferta p2;
    for(int j = 0; j < M; j++){
        fp.read((char *) &p2, sizeof(oferta));
        InsertHashOfertas(HashOfertas, p2, sizeO);
    };
    fp.close();


    // CREACION Y USO DE LA LISTA DE ARREGLOS    


    tListaArreglo Grupos_Equivalentes; // Esta lista va a contener en cada posicion un arreglo que contiene todos los codigos de un 
    // "grupo equivalente", esto para rellenar el campo ID_Grupo de los structs que contienen la informacion de los productos
    int cont = 0;
    bool flag = true;
    //Grupos_Equivalentes.moveToStart();
    while(flag){
        int cod = productos[cont];
        int keyProducto = HashP_getKey(HashProductos, cod, sizeP);
        if(tieneOferta(HashOfertas, cod, sizeO)){
            Grupos_Equivalentes.nuevoGrupo();
            Grupos_Equivalentes.insertElem(cod, 0);

            HashProductos[keyProducto].info.ID_Grupo = Grupos_Equivalentes.getPos();

            for(int y = 0; y < 10; y++){
                if(HashOfertas[keyProducto].info.productos_equivalentes[y] != -1){
                    int elem = HashOfertas[keyProducto].info.productos_equivalentes[y];
                    
                    Grupos_Equivalentes.insertElem(elem, y + 1);

                    int key = HashP_getKey(HashProductos, elem, sizeP);
                    HashProductos[key].info.ID_Grupo = Grupos_Equivalentes.getPos();  
                };
            };
            flag = false;
        }
        else {
            HashProductos[keyProducto].info.ID_Grupo = VACIA;
        };
        cont++;
    };

    flag = true;
    while(flag){
        int cod = productos[cont];
        int keyProducto = HashP_getKey(HashProductos, cod, sizeP);
        bool ElementoRepetido = false;

        if(tieneOferta(HashOfertas, cod, sizeO)){
            //Grupos_Equivalentes.moveToStart();
            int ID = 0; 
            while(ID < Grupos_Equivalentes.getListSize() && !ElementoRepetido){
                int* GrupoActual = Grupos_Equivalentes.getArreglo();
                if(CodigoEnGrupo(cod, GrupoActual)){
                    ElementoRepetido = true; // Si encuentra un elemento que ya tiene su grupo equivalente sale del while
                };
                //Grupos_Equivalentes.next();
            };

            if(!ElementoRepetido){      // aqui entra solo si buscó en toda la lista y no encontro al elemento, por lo que se crea un nuevo arreglo de 
                Grupos_Equivalentes.nuevoGrupo();       // grupos equivalentes
                Grupos_Equivalentes.insertElem(cod, 0);

                HashProductos[keyProducto].info.ID_Grupo = ID;   
                for(int y = 0; y < 10; y++){
                    if(HashOfertas[cod].info.productos_equivalentes[y] != -1){
                        int elem = HashOfertas[cod].info.productos_equivalentes[y];
                        HashProductos[elem].info.ID_Grupo = ID;
                        Grupos_Equivalentes.insertElem(elem, y + 1);
                    };
                };
            };
        }
        else {
            HashProductos[keyProducto].info.ID_Grupo = VACIA; // En caso de no tener oferta el campo ID_Grupo del producto quedara con valor -1
        };
        cont++;
        if(cont == N) flag = false;
    };
    


    int n_gruposEquivalentes = Grupos_Equivalentes.getListSize();
    Grupos_Equivalentes.~tListaArreglo(); // dado que la lista no sera de utilidad de aqui en mas, se elimina

    int ST, DT, MT, n_clientes, n_compras, cod_compra; // ST= suma total; DT= descuento total; MT= monto total(ST-DT)
    ST = DT = MT = 0;

    fstream archiv;
    archiv.open("compras.txt", ios::in);
    if (! archiv.is_open()){
        log("Error al abrir el archivo, el programa se cerrara");
        exit(1);
    };

    fstream out;
    out.open("boletas.txt", ios::out);
    if (! out.is_open()){
        log("Error al abrir el archivo, el programa se cerrara");
        exit(1);
    };


    archiv >> n_clientes;
    int contadores[n_gruposEquivalentes];
    for (int i = 0; i < n_gruposEquivalentes; i++){  // se inicia un arreglo que en cada posicion será el contador de un grupo equivalente. 
        contadores[i] = 0;
    };
    
    for(int p = 0; p < n_clientes; p++){
        archiv >> n_compras;
        for(int q = 0; q < n_compras; q++){
            archiv >> cod_compra;

            int keyProducto = HashP_getKey(HashProductos, cod_compra, sizeP);

            ST += HashProductos[keyProducto].info.precio; // Aqui se suma el precio del producto a la suma total

            if(tieneOferta(HashOfertas, cod_compra, sizeO)){ // Si el producto tiene oferta se buscara a que grupo pertenece y se aumentara
                TipoProductoInfo ProductoInfo = HashP_getInfo(HashProductos, cod_compra, sizeP); // el contador en esa posicion.
                TipoOfertaInfo OfertaInfo = HashO_getInfo(HashOfertas, cod_compra, sizeO);
                int Cantidad_Descuento = OfertaInfo.cantidad_descuento;
                if(ProductoInfo.ID_Grupo != VACIA){
                    contadores[ProductoInfo.ID_Grupo - 1]++;
                }
                if(contadores[ProductoInfo.ID_Grupo - 1] == Cantidad_Descuento){ // si el contador llega a la cantidad de productos necesarios
                    DT += OfertaInfo.descuento;       // para aplicar descuento, este se sumara al descuento total y el contador se reiniciara
                    contadores[ProductoInfo.ID_Grupo - 1] = 0;
                };
            };
        };

        MT = ST - DT;
        out << "Cliente:   Total de la compra: " <<endl;  // Añade al archivo de boletas la linea del cliente p
        out << p<< "            "<< MT <<endl;


        DT = MT = ST = 0;
        for (int i = 0; i < n_gruposEquivalentes; i++){  // Se reinician todas la variables en 0 para comenzar el siguiente ciclo
            contadores[i] = 0;
        };
    };

    archiv.close(); 
    out.close();
};