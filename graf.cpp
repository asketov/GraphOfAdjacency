#include <iostream>
#include <fstream>
#include <string>
 
/*Повышенный.  Граф  представляется  двумя  способами  (матрицей 
смежности  или  весов  и  списками  смежности),  для  тестирования  программы 
требуется создать файлы с описанием графа одним способом (только матрицей 
или только списками), при считывании данных из файла заполнять параллельно 
обе структуры хранения (и реализующую представление матрицы смежности 
(весов), и реализующую представление списков смежных вершин). При выборе 
структур хранения руководствоваться требованием разумной экономии памяти. */
 
/*13. Имеются  n  городов.  Некоторые  из  них  соединены  двусторонними 
дорогами известной длины. Определить, в каком из городов компании нужно 
построить  склад,  чтобы  доставка  товаров  по  городам  осуществлялась  с 
наименьшими транспортными расходами. Считать, что товары распределяются 
по городам равномерно и дорожные условия одинаковые. */
 
/*В файлах хранится матрица смежности, может храниться так и обычная матрица(текстовый файл)*/
 
using namespace std;
 
struct node
{
    int number_node;
    int length;
    struct node* next;
}; // 1 элемент списка смежности
 
 
 
class Matrix  //треугольная матрица смежности
{
private:
    int** matrix;
    int n;
public:
    Matrix(int n)
    {
        int i;
        this->n = n;
        matrix = new int* [n];
        for (i = 1; i < n; i++)
        {
            matrix[i] = new int[i];
        }
    }
    ~Matrix()
    {
        for (int i = 1; i < n; i++)
        {
            delete[]matrix[i];
        }
        delete[]matrix;
    }
    int** get_matrix()
    {
        return matrix;
    }
    void add_el(int lenght, int i, int j)
    {
        matrix[i][j] = lenght;
    }
    void print_matrix()
    {
        for (int i = 1; i < n; i++)
        {
            for (int j = 0; j < i; j++)
            {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};
 
int** matrix_adjacencies_m(int** matrix, int n)
{
    int i, j, min = 0, Imin = 0;
    int** A;
    A = new int* [n]; //матрица кратчайших путей между всеми парами вершин
    for (i = 0; i < n; i++)
    {
        A[i] = new int[n];
    }
    for (i = n - 2; i >= 0; i--)
    {
        for (j = i; j >= 0; j--)
        {
            A[i + 1][j] = matrix[i + 1][j];
            A[j][i + 1] = matrix[i + 1][j];
        }
    }
    for (i = 0; i < n; i++)
    {
        A[i][i] = 0;
    }
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    return A;
}
 
class Spisok
{
private:
    struct node** spisok;
    int n;
public:
    node** get_spisok()
    {
        return spisok;
    }
    Spisok(int n)
    {
        this->n = n;
        spisok = new node * [n];
        for (int i = 0; i < n; i++)
        {
            spisok[i] = NULL;
        }
    }
    ~Spisok()
    {
        struct node* l;
        for (int i = 0; i < n; i++)
        {
            while (spisok[i] != NULL)
            {
                l = spisok[i]->next;
                delete spisok[i];
                spisok[i] = l;
            }
        }
        delete[]spisok;
    }
    void add_el(int length, int i, int j)
    {
        if (length > 0)
        {
            if (spisok[i] == NULL)
            {
                add_first(i, length, j);
            }
            else
            {
                add_new(i, length, j);
            }
            if (spisok[j] == NULL)
            {
                add_first(j, length, i);
            }
            else
            {
                add_new(j, length, i);
            }
        }
    }
    void add_first(int i, int length, int j)
    {
        spisok[i] = new struct node;
        spisok[i]->length = length;
        spisok[i]->number_node = j;
        spisok[i]->next = NULL;
    }
    void add_new(int i, int length, int j)
    {
        struct node* l;
        l = spisok[i];
        spisok[i] = new struct node;
        spisok[i]->length = length;
        spisok[i]->number_node = j;
        spisok[i]->next = l;
    }
};
int** matrix_adjacencies_s(struct node** spisok, int n)
{
    int i;
    int** A;
    struct node* l;
    A = new int* [n]; //матрица кратчайших путей между всеми парами вершин
    for (i = 0; i < n; i++)
    {
        A[i] = new int[n];
    }
    for (i = 0; i < n; i++)
    {
        if (spisok[i] != NULL)
        {
            l = spisok[i];
            while (l->next != NULL)
            {
                if (l->number_node >= i)
                {
                    A[i][l->number_node] = l->length;
                    A[l->number_node][i] = l->length;
                }
                l = l->next;
            }
            A[i][l->number_node] = l->length;
            A[l->number_node][i] = l->length;
        }
    }
    return A;
}
 
 
int main()
{
    setlocale(0, "Russian");
    string fil,line;
    int n, i, j, el, min1, Imin, max, k;
    char choise;
    int** A;
    cout << "Введите название файла:";
    cin >> fil;
    cout << "Введите количество вершин:";
    cin >> n;
    
    class Matrix matrix(n);
    class Spisok spisok(n);
    ifstream file(fil);
    if (file.is_open())
    {
        for (i = 1; i < n; i++)
        {
            getline(file, line);
            for (j = 0; j < i; j++)
            {
                file >> el;
                matrix.add_el(el, i, j);
                spisok.add_el(el, i, j);
            }
        }
    }
    file.close();
    cout << "С помощью какой структуры хранения вы хотите найти центр графа?" << endl;
    cout << "1.Матрица весов" << endl;
    cout << "2.Списки смежности" << endl;
    cin >> choise;
    if (choise == 1)
    {
        A = matrix_adjacencies_m(matrix.get_matrix(),n);
    }
    else A = matrix_adjacencies_s(spisok.get_spisok(), n);
    for (i = 0; i < n; i++)
        A[i][i] = 0;
    for (k = 0; k < n; k++)
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                if (A[i][k] + A[k][j] < A[i][j]) /* если путь через k короче*/
                {
                    A[i][j] = A[i][k] + A[k][j]; /* запоминаем длину пути*/
                }
    cout << "Матрица кратчайших путей между парами вершин:" << endl;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
 
    for (i = 0; i < n; i++)
    {
        max = 0;
        for (j = 0; j < n; j++)
        {
            if (A[j][i] > max)
            {
                max = A[j][i];
            }
        }
        if (i == 0)
        {
            min1 = max;
            Imin = i;
        }
        else if (max < min1)
        {
            min1 = max;
            Imin = i;
        }
    }
    cout << "Центром графа является вершина под номером:" << Imin + 1 << endl;
    for (i = 0; i < n; i++)
    {
        delete[]A[i];
    }
    delete[]A;
}

