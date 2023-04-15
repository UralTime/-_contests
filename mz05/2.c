/*
 *          Некоторые функции для работы с бинарным
 *      неупорядоченным деревом для элементов типа  int
 */
#ifndef _TREE_H_
#define _TREE_H_
#include <stdio.h>


// описание вершины дерева
struct TreeNode
{
        int  value;
        TreeNode *left,*right;
        TreeNode() { value = 0; left = right = NULL; }
};

// класс бинарное дерево
class  Tree
{
    public:
        TreeNode *root;   // указатель на корень дерева

        // Конструктор пустого дерева
        Tree () { root = NULL; }

        // Деструктор
        ~Tree ();

        // Конструктор заполнения дерева числами из открытого файла.
        //     f   -  указатель на файл,
        Tree (FILE *f);

};

    
        //  Распечатка поддерева в файл.
        //     f   -  указатель на файл,
        //    pos  -  указатель на корень поддерева.

        void  T_Print (FILE *f, TreeNode *pos);

       /*  Примечание:
        *     Дерево распечатывается "лежа на левом боку", т.е.
        *  корень стоит слева, правая ветвь идет направо-вверх,
        *  левая ветвь идет направо-вниз.
        */

#endif

#include "stdio.h"

int preobr(TreeNode *pos)
{
if (pos == nullptr) return 0;
int result = 1 + preobr(pos -> left) + preobr(pos -> right);
pos->value = result;
return result;
}


int  main(void)
{
    FILE *fin, *fout;

    // открываем и проверяем файлы
    if (!(fout = fopen("tree.res","w")))
        { printf("\n Cannot open file tree.res\n"); return -1; }
    if (!(fin = fopen("tree.dat","r")))
        { printf("\n Cannot open file tree.dat\n"); return -1; }

    // заполняем дерево числами из файла - работает конструктор с параметрами:
    Tree tr(fin);
    fclose(fin);

    // распечатаем исходное дерево на экране
    printf("\nThe tree is :\n");
    T_Print(stdout,tr.root);
    T_Print(fout,tr.root);

/* сюда следует вставить вызов Вашей функции для решения поставленной задачи */
	preobr(tr.root);
//	preobr(tr.root,0);
/*     
        int k;
        printf("\nВведи k=");scanf("%d",&k);
        preobr(tr.root,k,0);
*/

    // распечатаем преобразованное дерево в файл

    fprintf(fout,"\n======================\n");
    T_Print(fout,tr.root);

    // закрываем файл результата
    fclose(fout);
    return 0;
}

