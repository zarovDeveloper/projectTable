#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox> //подключаем библиотеку с окошками сообщений
#include <bits/stdc++.h> //библиотека для С++, где уже включены все базовые функиции std, string и т.д.
#include <QTime> //таймер
#include <QFileDialog> //для открытия файлов
#include <QTextStream>

using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_AddRow_clicked();

    void on_pushButton_AddColum_clicked();

    void on_pushButton_DelRow_clicked();

    void on_pushButton_DelColum_clicked();

    void on_spinBox_Row_valueChanged(int arg1);

    void on_spinBox_search_valueChanged();

    void on_pushButton_Random_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_pushButton_sort_bubble_clicked();

    void on_pushButton_Sum_clicked();

    void on_pushButton_mul_clicked();

    void on_pushButton_max_clicked();

    void on_pushButton_min_clicked();

    void on_pushButton_avr_clicked();

    void on_pushButton_sort_comb_clicked();

    void on_pushButton_sort_gnome_clicked();

    void on_pushButton_sort_quick_clicked();

    void on_pushButton_sort_bogo_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_deleteDup_clicked();

    void on_lineEdit_search_textChanged();

//    void on_pushButton_openTxt_clicked();

//    void on_pushButton_saveTxt_clicked();

    void on_pushButton_openBin_clicked();

    void on_pushButton_saveBin_clicked();

//    void saveTxt();

//    void openTxt();

    void openBin();

    void saveBin();

private:

    Ui::MainWindow *ui;


    QTime timer; //таймер


    //Флаги


    bool no_auto_change = true;//признак ручного ввода
    bool flag;                //
    bool error = true;        //    флаги на проверки
    bool error_value = false; //


    //Функции


    void math_sum(); //функиця суммы

    void math_avr(); //фукнция среднего значения

    void math_max(); //фукнция максимального значения

    void math_min(); //фукнция минимального значения

    void math_mul(); //функция произведения

    void sort_bubble(); //функция сортировкой пузырек

    void sort_comb(); //функция сортировкой расчесткой

    void sort_gnome(); //функция гномьей сортировкой

    void sort_quick(); //функция быстрой сортировки

    void sort_bogo(); //функция обезьяньей сортировкой

    void search_enum(); //функция поиска

    void search_binary(); //функция бинарного поиска

    void remove_duplicate(); //функция удаления дубликатов


    //Сортировки


    int* bubble(int arr[], int size) //функция сортировки пузырек
    {
        for (int i=0; i<size; i++)
            for (int j=0; j<(size-1); j++)
                if (arr[j] > arr[j+1])
                    swap(arr[j], arr[j + 1]);

        return arr;
    }

    int* comb(int arr[], int size) //функция сортировки расчетской
    {
        // переменные для счета
        double factor = 1.2473309; //фактор уменьшения
        int step = size - 1; //шаг сортировки

        while (step >= 1)
        {
            for (int i=0; i+step<size; i++)
            {
                if (arr[i] > arr[i + step])
                    swap(arr[i], arr[i + step]);
            }
            step /= factor;
        }

        return arr;
    }

    int* gnome(int arr[], int size) //функция гномьей сортировки
    {
        int step = 0; //шаг сортировки

        while (step + 1 < size)
        {
            if (arr[step] <= arr[step+1])
                step++;
            else
            {
                swap(arr[step], arr[step+1]);
                step--;

                if (step<0)
                    step = 0;
            }
        }

        return arr;
    }

    enum SortOrder //функиця сортировки
    {
        ASC,    //по возрастанию
        DESC    //по убыванию
    };

    void *bogo(int arr[], int size, SortOrder sortOrder) //функция обезьяньей сортировки
    {
        while (!isSorted(arr, size, sortOrder))
            shuffle(arr, size);

        return arr;
    }



    //Доп. фкнкции



    void *shuffle(int arr[], int size) //тасовка
    {
        while (size > 1)
            {
                int r = rand() % size;
                size -= 1;
                swap(arr[r], arr[size]);
            }
        return arr;
    }

    bool isElementsSorted(int a, int b, SortOrder sortOrder)
    {
        if (sortOrder == ASC)
        {
            return a <= b;
        }
        else
        {
            return a >= b;
        }
    }

    bool isSorted(int arr[], int size, SortOrder sortOrder) //проверка сортировки массива
    {
        for (int i = 0; i < size - 1; i++)
            {
                if (!isElementsSorted(arr[i], arr[i + 1], sortOrder))
                    return false;
            }

            return true;
    }

    int* clearArr(int arr[]) //чистка массива
    {
        delete [] arr;
        arr = nullptr;
        return arr;
    }

    struct comp //структура для сортировки по убыванию
    {
        bool operator()(int const &i, int const &j) const {
            return i > j;
        }
    };
};

#endif // MAINWINDOW_H
