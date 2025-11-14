### Ejercicio 1.b
En este caso se precisa que nuestra estructura sea un monoide, pues precisamos:
- La existencia del neutro para operar en el arbol y los casos que tenemos que
  rellenar con elementos para tener un arbol completo.
- La asociatividad, por la forma en la que se deben realizar las operaciones
  sobre la estructura de rope.

### Ejercicio 2.b
En este caso necesitamos que nuestro conjunto de valores cumpla con las 
propiedades de un monoide al igual que en el caso anterior, pero
adicionalmente tambien precisamos que el conjunto de actualizaciones
pendientes tambien forme un monoide. Esto se debe a que necesitamos que 
las actualizaciones lazy pueden acumularse y combinarse entre sí antes
de ser aplicadas a los nodos del árbol. La asociatividad permite que
múltiples actualizaciones se puedan componer en cualquier orden, 
mientras que el elemento neutro garantiza que las operaciones de
propagación no alteren los valores cuando no hay actualizaciones 
pendientes. Esto es fundamental para el correcto funcionamiento 
en caso de que la propagacion en el arbol sea necesaria.
