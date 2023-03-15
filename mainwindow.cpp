#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidget_ans_search_num->setVisible(false); //скрыть listWidget
    ui->radioButton_increasing->click(); //иммитация клика мышки на кнопку возрастания

    setWindowTitle("Table Project"); //изменили название программы
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Кнопки

void MainWindow::on_pushButton_AddRow_clicked()//кнопка добавить строку
{
    ui->spinBox_Row->setValue(ui->spinBox_Row->value()+1);
}

void MainWindow::on_pushButton_AddColum_clicked()//кнопка добивать стобец
{
    ui->tableWidget->setColumnCount(ui->tableWidget->columnCount()+1);
}

void MainWindow::on_pushButton_DelRow_clicked() //кнопка убрать строку
{
    ui->spinBox_Row->setValue(ui->spinBox_Row->value()-1);
}

void MainWindow::on_pushButton_DelColum_clicked() //кнопка убрать столбец
{
    if (ui->tableWidget->columnCount() != 1)
        ui->tableWidget->setColumnCount(ui->tableWidget->columnCount()-1);
}

void MainWindow::on_pushButton_Sum_clicked() //кнопка суммы
{
    timer.start(); //таймер

    math_sum();

    ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
}

void MainWindow::on_pushButton_mul_clicked() //кнопка произведения
{
    timer.start(); //таймер

    math_mul();

    ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
}

void MainWindow::on_pushButton_max_clicked() //кнопка нахождения максимального значения
{
    timer.start(); //таймер

    math_max();

    ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
}

void MainWindow::on_pushButton_min_clicked() //кнопка нахождения минимального значения
{
    timer.start(); //таймер

    math_min();

    ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
}

void MainWindow::on_pushButton_avr_clicked() //кнопка нахождения среднего значения
{
    timer.start(); //таймер

    math_avr();

    ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
}

void MainWindow::on_pushButton_sort_bubble_clicked() //кнопка сортировки пузырком
{
    sort_bubble();
}

void MainWindow::on_pushButton_sort_comb_clicked() //кнопка сортировки расчетской
{
    sort_comb();
}

void MainWindow::on_pushButton_sort_gnome_clicked() //кнопка гномьей сортировки
{
    sort_gnome();
}

void MainWindow::on_pushButton_sort_quick_clicked() //кнопка быстрой сортировки
{
    sort_quick();
}

void MainWindow::on_pushButton_sort_bogo_clicked() //кнопка обезьяньей сортировки
{
    sort_bogo();
}

void MainWindow::on_pushButton_search_clicked() //кнопка поиска
{
    timer.start(); //таймер

    no_auto_change = false;

    error_value = false;

    int row = ui->tableWidget->rowCount();
    int *arr_table = new int[row]; //массив для значений

    for (int i = 0; i < row; i++) //проверка на существования значений в 1 столбце
    {
        if (ui->tableWidget->item(i, 0)!=nullptr)
        {//есть ячейка
            arr_table[i] = ui->tableWidget->item(i, 0)->text().toInt(&flag); //создание массива
            if (flag)
                ui->tableWidget->item(i,0)->setBackground(Qt::white);
            else
            {
                ui->tableWidget->item(i,0)->setBackground(Qt::magenta);
                error_value = true; //хотя бы в одной ячейки некорректные данные
            }
        }
        else
        { // нет ячейки
            QTableWidgetItem * ti; //создали указатель
            ti =  new QTableWidgetItem(); //выделяем память
            ui->tableWidget->setItem(i,0,ti); //помещаем ячейку в таблицу
            ui->tableWidget->item(i,0)->setBackground(Qt::darkGray);
            error_value = true; //хотя бы одной ячейки нет вообще
        }
        if (error_value)
            break;
    }

    if (error_value)
        QMessageBox::information(this, "Ошибка", "Некоректные значения", QMessageBox::Ok);
    else
    {
        bool flag_sort = isSorted(arr_table, row, ASC); //автоматом на перебор по возрастанию

        if (flag_sort)
            search_binary();
        else
            search_enum();
    }
    no_auto_change = true;

    ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
}

void MainWindow::on_pushButton_deleteDup_clicked() //кнопка удаления дубликатов
{
    timer.start(); //таймер

    if (ui->spinBox_Row->value() >= 200000)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Предупреждение", "Данная операция будет выполняться более 5 секунд. \nВы уверены?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            remove_duplicate();
            ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
        }
        else
            ui->label_time->setText("Отмена"); //вывод времени операции в мс
    }
    else
    {
        remove_duplicate();
        ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
    }
}


//Изменение, заполнение таблицы


void MainWindow::on_spinBox_Row_valueChanged(int arg1) //изменили значение спинбокса
{
    ui->tableWidget->setRowCount(arg1);
    ui->label_ans->clear();
}

void MainWindow::on_spinBox_search_valueChanged() //изменили значение спинбокса для поиска
{
    ui->listWidget_ans_search_num->clear();
    ui->label_ans_search_entry->clear();
    ui->label_search->clear();
    ui->listWidget_ans_search_num->setVisible(false);
    ui->label_ans->clear();
}

void MainWindow::on_pushButton_Random_clicked() //заполнение случайными числами
{
    timer.start(); //таймер

    no_auto_change = false;

    if (ui->spinBox_Row->value() >= 600000)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Предупреждение", "Данная операция будет выполняться более 3 секунд. \nВы уверены?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            int row = ui->tableWidget->rowCount();
            int colum = ui->tableWidget->columnCount();

            int value;

            for (int i = 0; i < row; i++)
                for (int j = 0;j < colum; j++)
                {
                    if (ui->tableWidget->item(i,j) == nullptr)
                    {
                        QTableWidgetItem * ti; //создали указатель
                        ti =  new QTableWidgetItem(); //выделяем память
                        ui->tableWidget->setItem(i,j,ti); //помещаем ячейку в таблицу
                    }
                    ui->tableWidget->item(i,j)->setBackgroundColor(Qt::white);

                    value = rand() % 201 - 100; // от -100 до 100
                    ui->tableWidget->item(i,j)->setText(QString::number(value));
                }

            ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
        }
        else
            ui->label_time->setText("Отмена"); //вывод времени операции в мс
    }
    else
    {
        int row = ui->tableWidget->rowCount();
        int colum = ui->tableWidget->columnCount();

        int value;

        for (int i = 0; i < row; i++)
            for (int j = 0;j < colum; j++)
            {
                if (ui->tableWidget->item(i,j) == nullptr)
                {
                    QTableWidgetItem * ti; //создали указатель
                    ti =  new QTableWidgetItem(); //выделяем память
                    ui->tableWidget->setItem(i,j,ti); //помещаем ячейку в таблицу
                }
                ui->tableWidget->item(i,j)->setBackgroundColor(Qt::white);

                value = rand() % 201 - 100; // от -100 до 100
                ui->tableWidget->item(i,j)->setText(QString::number(value));
            }

        ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
    }

    no_auto_change = true;
}

void MainWindow::on_tableWidget_cellChanged(int row, int column) //изменили ячейку
{
    if (error) //один раз показать ошибку
        if (no_auto_change)
        {
            int num = ui->tableWidget->item(row,column)->text().toInt(&flag);
            ui->label_ans->clear();
            if (flag)
                if (num <= 100 and num >= -100) //проверка на область допустимых значений
                    ui->tableWidget->item(row,column)->setBackground(Qt::white);
                else
                {
                    error = false;
                    QMessageBox::information(this, "Ошибка", "Допустимые значения от -100 до 100", QMessageBox::Ok);
                    ui->tableWidget->item(row,column)->setBackground(Qt::magenta);
                }
            else
            {
                ui->label_ans->clear();
                error = false;
                QMessageBox::information(this, "Ошибка", "Введены некоректные значения.", QMessageBox::Ok);
                ui->tableWidget->item(row,column)->setBackground(Qt::red);

            }
        }

    error = true;

    //очистка всех значений
    //ui->label_ans->clear();
    ui->label_ans_search_entry->clear();
    ui->label_search->clear();
    ui->listWidget_ans_search_num->clear();
    ui->listWidget_ans_search_num->setVisible(false);
}

void MainWindow::on_lineEdit_search_textChanged() //динамическое изменение ошибки в line edit search
{
    int num = ui->lineEdit_search->text().toInt(&flag);

    if ((!flag) or (num > 100) or (num < -100))
        ui->lineEdit_search->setStyleSheet("color: red;");
    else
        ui->lineEdit_search->setStyleSheet("color: black;");
}


//Математика


void MainWindow::math_sum() //сумма
{
    no_auto_change = false;

    ui->label_ans->clear(); //удаление старых значений

    int row = ui->tableWidget->rowCount();
    int colum = ui->tableWidget->columnCount();

    int sum = 0, count = 0; //переменные для счета

    error = false; //флаг для ошибок
    bool error_val = false;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < colum; j++)
            if (ui->tableWidget->item(i,j)!=nullptr) //проверка существования ячейки
            {//есть ячейка
                if ((ui->tableWidget->item(i,j)->text().toInt(&flag)) > 100 or (ui->tableWidget->item(i,j)->text().toInt(&flag) < -100))
                {
                    error = true;
                    error_val = true;
                }
                else
                {
                    sum += ui->tableWidget->item(i,j)->text().toInt(&flag);
                    count += 1;
                }
                if (flag)
                    if (error_val)
                    {
                        ui->tableWidget->item(i,j)->setBackground(Qt::red);
                        error_val = false;
                    }
                    else
                        ui->tableWidget->item(i,j)->setBackground(Qt::white);
                else
                {
                    error = true;
                    ui->tableWidget->item(i,j)->setBackground(Qt::red);
                }
            }
            else
            { // нет ячейки
                error = true; //ошибка
                QTableWidgetItem * ti; //создали указатель
                ti =  new QTableWidgetItem(); //выделяем память
                ui->tableWidget->setItem(i,j,ti); //помещаем ячейку в таблицу
                ui->tableWidget->item(i,j)->setBackground(Qt::blue); //красим
            }

    if (error) //есть ошибки
        QMessageBox::information(this,"Ошибка","Введены некоректные значения.",QMessageBox::Ok);
    else //ошибок нет
        if (count != 0) //проверка на количество значений
            ui->label_ans->setNum(sum); // значений больше нуля
        else // значений 0
            QMessageBox::information(this, "Ошибка", "Нет значений", QMessageBox::Ok);

    no_auto_change=true;
}

void MainWindow::math_avr() //среднее значение
{
    no_auto_change = false;

    ui->label_ans->clear(); //удаление старых значений

    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();

    int count = 0, sum = 0, avr = 0, avr_ost = 0; //переменные для счета

    error = false; //ошибок нет
    bool error_val = false;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            if (ui->tableWidget->item(i, j)!=nullptr) //проверка существования
            {// есть ячейка
                if ((ui->tableWidget->item(i,j)->text().toInt(&flag)) > 100 or (ui->tableWidget->item(i,j)->text().toInt(&flag) < -100))
                {
                    error_val = true;
                    error = true;
                }
                else
                {
                    sum += ui->tableWidget->item(i,j)->text().toInt(&flag);
                    count += 1;
                }
                if (flag) //правильное значение
                    if (error_val)
                    {
                        ui->tableWidget->item(i,j)->setBackground(Qt::red);
                        error_val = false;
                    }
                    else
                        ui->tableWidget->item(i,j)->setBackground(Qt::white);
                else
                { //неправильное значение
                    error = true; //ошибка
                    ui->tableWidget->item(i, j)->setBackground(Qt::red);
                }
            }
            else
            {// нет ячейки
                error = true; //ошибка
                QTableWidgetItem * ti; //создали указатель
                ti = new QTableWidgetItem(); //выделили память
                ui->tableWidget->setItem(i, j, ti); //помещаем ячейку в таблицу
                ui->tableWidget->item(i, j)->setBackground(Qt::blue); //красим
            }

    if (error) //проверка на ошибки
        QMessageBox::information(this, "Ошибка", "Введены некоректные значения", QMessageBox::Ok);
    else
        if (count != 0) //проверка на количество значений
        {// значений больше нуля
            avr = sum / count; //счет целой части
            avr_ost = sum % count; //счет нецелой части
            if (avr_ost < 0)
            {//если остаток отрицательный
                avr_ost = -avr_ost;
                ui->label_ans->setText("-" + QString::number(avr) + "." + QString::number(avr_ost));
            }
            ui->label_ans->setText(QString::number(avr) + "." + QString::number(avr_ost));
        }
        else // значений 0
            QMessageBox::information(this, "Ошибка", "Нет значений", QMessageBox::Ok);

    no_auto_change = true;
}

void MainWindow::math_max() //максимальное значение
{
    ui->label_ans->clear(); //удаление старых значений

    no_auto_change = false;

    int row = ui->tableWidget->rowCount();
    int colum = ui->tableWidget->columnCount();

    int max = -101, count = 0; //переменные для счета

    error = false; //ошибок нет
    bool error_val = false;

    for (int i = 0;i < row; i++)
        for (int j = 0;j < colum; j++)
            if (ui->tableWidget->item(i,j)!=nullptr) //проверка существования ячейки
            {//есть ячейка
                if ((ui->tableWidget->item(i,j)->text().toInt(&flag)) > 100 or (ui->tableWidget->item(i,j)->text().toInt(&flag) < -100))
                {
                    error_val = true;
                    error = true;
                }
                else
                    if ((ui->tableWidget->item(i,j)->text().toInt(&flag)) > max)
                    {
                        max = ui->tableWidget->item(i,j)->text().toInt(&flag);
                        count += 1;
                        if (max == 100)
                            break;
                    }
                if (flag)
                    if (error_val)
                    {
                        ui->tableWidget->item(i,j)->setBackground(Qt::red);
                        error_val = false;
                    }
                    else
                        ui->tableWidget->item(i,j)->setBackground(Qt::white);
                else
                {
                    error = true;
                    ui->tableWidget->item(i,j)->setBackground(Qt::red);
                }
            }
            else
            { // нет ячейки
                error = true; //ошибка
                QTableWidgetItem * ti; //создали указатель
                ti =  new QTableWidgetItem(); //выделяем память
                ui->tableWidget->setItem(i,j,ti); //помещаем ячейку в таблицу
                ui->tableWidget->item(i,j)->setBackground(Qt::blue);
            }

    if (error)
        QMessageBox::information(this,"Ошибка","Введены некоректные значения.",QMessageBox::Ok);
    else
        if (count != 0) //проверка на количество значений
            ui->label_ans->setNum(max); // значений больше нуля
        else // значений 0
            QMessageBox::information(this, "Ошибка", "Введены некоректные значения", QMessageBox::Ok);

    no_auto_change=true;
}

void MainWindow::math_min() //минимальное значение
{
    ui->label_ans->clear(); //удаление старых значений

    no_auto_change = false;

    int row = ui->tableWidget->rowCount();
    int colum = ui->tableWidget->columnCount();

    int min = 101, count = 0; //переменные для счета

    error = false; //ошибок нет
    bool error_val = false;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < colum; j++)
            if (ui->tableWidget->item(i,j)!=nullptr) //проверка существования ячейки
            {//есть ячейка
                if ((ui->tableWidget->item(i,j)->text().toInt(&flag)) > 100 or (ui->tableWidget->item(i,j)->text().toInt(&flag) < -100))
                {
                    error_val = true;
                    error = true;
                }
                else
                    if ((ui->tableWidget->item(i,j)->text().toInt(&flag)) < min)
                    {
                        min = ui->tableWidget->item(i,j)->text().toInt(&flag);
                        count += 1;
                        if (min == -100)
                            break;
                    }
                if (flag)
                    if (error_val)
                    {
                        ui->tableWidget->item(i,j)->setBackground(Qt::red);
                        error_val = false;
                    }
                    else
                        ui->tableWidget->item(i,j)->setBackground(Qt::white);
                else
                {
                    error = true;
                    ui->tableWidget->item(i,j)->setBackground(Qt::red);
                }
            }
            else
            { // нет ячейки
                error = true; //ошибка
                QTableWidgetItem * ti; //создали указатель
                ti =  new QTableWidgetItem(); //выделяем память
                ui->tableWidget->setItem(i,j,ti); //помещаем ячейку в таблицу
                ui->tableWidget->item(i,j)->setBackground(Qt::blue);
            }

    if (error)
        QMessageBox::information(this,"Ошибка","Введены некоректные значения.",QMessageBox::Ok);
    else
        if (count != 0) //проверка на количество значений
            ui->label_ans->setNum(min); // значений больше нуля
        else // значений 0
            QMessageBox::information(this, "Ошибка", "Введены некоректные значения", QMessageBox::Ok);

    no_auto_change=true;
}

void MainWindow::math_mul() //произведение
{
    no_auto_change=false;

    ui->label_ans->clear(); //удаление старых значений

    int row = ui->tableWidget->rowCount();
    int colum = ui->tableWidget->columnCount();

    int mul = 1, count = 0;

    error = false; //флаг для ошибок
    bool error_val = false;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < colum; j++)
            if (ui->tableWidget->item(i,j)!=nullptr) //проверка существования ячейки
            {//есть ячейка
                if ((ui->tableWidget->item(i,j)->text().toInt(&flag)) > 100 or (ui->tableWidget->item(i,j)->text().toInt(&flag) < -100))
                {
                    error = true;
                    error_val = true;
                }
                else
                {
                    mul *= ui->tableWidget->item(i,j)->text().toInt(&flag);
                    count += 1;
                    if (mul == 0)
                        break;
                }
                if (flag)
                    if (error_val)
                    {
                        ui->tableWidget->item(i,j)->setBackground(Qt::red);
                        error_val = false;
                    }
                    else
                        ui->tableWidget->item(i,j)->setBackground(Qt::white);
                else
                {
                    error = true;
                    ui->tableWidget->item(i,j)->setBackground(Qt::red);
                }
            }
            else
            { // нет ячейки
                error = true; //ошибка
                QTableWidgetItem * ti; //создали указатель
                ti =  new QTableWidgetItem(); //выделяем память
                ui->tableWidget->setItem(i,j,ti); //помещаем ячейку в таблицу
                ui->tableWidget->item(i,j)->setBackground(Qt::blue);
            }

    if (error) //есть ошибки
        QMessageBox::information(this,"Ошибка","Введены некоректные значения.",QMessageBox::Ok);
    else //ошибок нет
        if (count != 0) //проверка на количество значений
            ui->label_ans->setNum(mul); //значений больше 1
        else // значений 0
            QMessageBox::information(this, "Ошибка", "Нет значений", QMessageBox::Ok);

    no_auto_change=true;
}


//Сортировки


void MainWindow::sort_bubble() //сортировка пузырком
{
    timer.start(); //запуск таймер

    no_auto_change = false;
    error_value = false;

    //узнаем значения строк и колонок
    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();

    //создаем динамический массив
    int *arr_table = new int [row]; //массив для данных из таблицы

    QTableWidgetItem * ti; //создали указатель

    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
        {
            if (ui->tableWidget->item(i, 0)!=nullptr)
            {//есть ячейка
                arr_table[i] = ui->tableWidget->item(i, 0)->text().toInt(&flag);
                if ((flag) and (arr_table[i] >= -100) and (arr_table[i] <= 100))
                    ui->tableWidget->item(i,0)->setBackground(Qt::white);
                else
                {
                    ui->tableWidget->item(i,0)->setBackground(Qt::magenta);
                    error_value = true; //ошибка значения
                }
            }
            else
            {
                ti = new QTableWidgetItem(); //выделили память
                ui->tableWidget->setItem(i,0,ti); //помещаем ячейку в таблицу
                ui->tableWidget->item(i,0)->setBackground(Qt::darkGray); //красим
            }
        }

    if (column != 1)//если ошибка в нескольких столбцах
        QMessageBox::information(this,"Ошибка","Для сортировки необходим 1 столбец", QMessageBox::Ok);
    else if (error_value)//если ошибка в значении
        QMessageBox::information(this,"Ошибка","Таблица содержит неверные данные.",QMessageBox::Ok);
    else if ((ui->radioButton_increasing->isChecked()) and isSorted(arr_table, row, ASC))
        return;
    else if ((ui->radioButton_decreasing->isChecked()) and isSorted(arr_table, row, DESC))
        return;
    else
    {//нет ошибок
        if (ui->spinBox_Row->value() >= 20000)
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Предупреждение", "Данная операция будет выполняться более 5 секунд. \nВы уверены?", QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
                if (ui->radioButton_increasing->isChecked())
                    bubble(arr_table, row); //пузырьковая сортировка
                else
                    sort(arr_table, arr_table + row, comp()); //быстрая сортировка по убыванию

                for (int i = 0; i < row; i++) //вывод массива в таблицу
                    ui->tableWidget->item(i,0)->setText(QString::number(arr_table[i]));

                ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
            }
            else
                ui->label_time->setText("Отмена"); //вывод времени операции в мс
        }
        else
        {
            if (ui->radioButton_increasing->isChecked())
                bubble(arr_table, row); //пузырьковая сортировка
            else
                sort(arr_table, arr_table + row, comp()); //быстрая сортировка по убыванию

            for (int i = 0; i < row; i++) //вывод массива в таблицу
                ui->tableWidget->item(i,0)->setText(QString::number(arr_table[i]));

            ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
        }
    }
    //чистим память
    arr_table = clearArr(arr_table);

    no_auto_change=true;
}

void MainWindow::sort_comb() //сортировка расчесткой
{
    timer.start(); //запуск таймер

    no_auto_change = false;
    error_value = false;

    //узнаем значения строк и колонок
    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();

    //создаем динамический массив
    int *arr_table = new int [row]; //массив для данных из таблицы

    QTableWidgetItem * ti; //создали указатель

    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
        {
            if (ui->tableWidget->item(i, 0)!=nullptr)
            {//есть ячейка
                arr_table[i] = ui->tableWidget->item(i, 0)->text().toInt(&flag);
                if ((flag) and (arr_table[i] >= -100) and (arr_table[i] <= 100))
                    ui->tableWidget->item(i,0)->setBackground(Qt::white);
                else
                {
                    ui->tableWidget->item(i,0)->setBackground(Qt::magenta);
                    error_value = true; //ошибка значения
                }
            }
            else
            {
                ti = new QTableWidgetItem(); //выделили память
                ui->tableWidget->setItem(i,0,ti); //помещаем ячейку в таблицу
                ui->tableWidget->item(i,0)->setBackground(Qt::darkGray); //красим
            }
        }

    if (column != 1) //если ошибка в нескольких столбцах
        QMessageBox::information(this,"Ошибка","Для сортировки необходим 1 столбец", QMessageBox::Ok);
    else if (error_value) //если ошибка в значении
        QMessageBox::information(this,"Ошибка","Таблица содержит неверные данные.",QMessageBox::Ok);
    else if ((ui->radioButton_increasing->isChecked()) and isSorted(arr_table, row, ASC))
        return;
    else if ((ui->radioButton_decreasing->isChecked()) and isSorted(arr_table, row, DESC))
        return;
    else
    {//нет ошибок
        if (ui->spinBox_Row->value() >= 600000)
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Предупреждение", "Данная операция будет выполняться более 3 секунд. \nВы уверены?", QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
                if (ui->radioButton_increasing->isChecked())
                    comb(arr_table, row); //сортировка расческтой
                else
                    sort(arr_table, arr_table + row, comp()); //быстрая сортировка по убыванию

                for (int i = 0; i < row; i++) //вывод массива в таблицу
                    ui->tableWidget->item(i,0)->setText(QString::number(arr_table[i]));

                ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
            }
            else
                ui->label_time->setText("Отмена"); //вывод времени операции в мс
        }
        else
        {
            if (ui->radioButton_increasing->isChecked())
                comb(arr_table, row); //сортировка расчесткой
            else
                sort(arr_table, arr_table + row, comp()); //быстрая сортировка по убыванию

            for (int i = 0; i < row; i++) //вывод массива в таблицу
                ui->tableWidget->item(i,0)->setText(QString::number(arr_table[i]));

            ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
        }
    }

    //чистим память
    arr_table = clearArr(arr_table);

    no_auto_change=true;
}

void MainWindow::sort_gnome() //гномья сортировка
{
    timer.start(); //запуск таймер

    no_auto_change = false;
    error_value = false;

    //узнаем значения строк и колонок
    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();

    //создаем динамический массив
    int *arr_table = new int [row]; //массив для данных из таблицы

    QTableWidgetItem * ti; //создали указатель

    //проверка на 1 столбец и существование значения
    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
        {
            if (ui->tableWidget->item(i, 0)!=nullptr)
            {//есть ячейка
                arr_table[i] = ui->tableWidget->item(i, 0)->text().toInt(&flag);
                if ((flag) and (arr_table[i] >= -100) and (arr_table[i] <= 100))
                    ui->tableWidget->item(i,0)->setBackground(Qt::white);
                else
                {
                    ui->tableWidget->item(i,0)->setBackground(Qt::magenta);
                    error_value = true; //ошибка значения
                }
            }
            else
            {
                ti = new QTableWidgetItem(); //выделили память
                ui->tableWidget->setItem(i,0,ti); //помещаем ячейку в таблицу
                ui->tableWidget->item(i,0)->setBackground(Qt::darkGray); //красим
            }
        }

    if (column != 1) //если ошибка в нескольких столбцах
        QMessageBox::information(this,"Ошибка","Для сортировки необходим 1 столбец", QMessageBox::Ok);
    else if (error_value) //если ошибка в значении
        QMessageBox::information(this,"Ошибка","Таблица содержит неверные данные.",QMessageBox::Ok);
    else if ((ui->radioButton_increasing->isChecked()) and isSorted(arr_table, row, ASC))
        return;
    else if ((ui->radioButton_decreasing->isChecked()) and isSorted(arr_table, row, DESC))
        return;
    else
    {//нет ошибок
        if (ui->spinBox_Row->value() >= 40000)
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Предупреждение", "Данная операция будет выполняться более 5 секунд. \nВы уверены?", QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
                if (ui->radioButton_increasing->isChecked())
                    gnome(arr_table, row); //гномья сортировка
                else
                    sort(arr_table, arr_table + row, comp()); //быстрая сортировка по убыванию

                for (int i = 0; i < row; i++) //вывод массива в таблицу
                    ui->tableWidget->item(i,0)->setText(QString::number(arr_table[i]));

                ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
            }
            else
                ui->label_time->setText("Отмена"); //вывод времени операции в мс
        }
        else
        {
            if (ui->radioButton_increasing->isChecked())
                gnome(arr_table, row); //сортировка гномья
            else
                sort(arr_table, arr_table + row, comp()); //быстрая сортировка по убыванию

            for (int i = 0; i < row; i++) //вывод массива в таблицу
                ui->tableWidget->item(i,0)->setText(QString::number(arr_table[i]));

            ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
        }
    }

    //чистим память
    arr_table = clearArr(arr_table);

    no_auto_change=true;
}

void MainWindow::sort_quick() //быстрая сортировка
{
    timer.start(); //запуск таймер

    no_auto_change = false;
    error_value = false;

    //узнаем значения строк и колонок
    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();

    //создаем динамический массив
    int *arr_table = new int [row]; //массив для данных из таблицы

    QTableWidgetItem * ti; //создали указатель

    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
        {
            if (ui->tableWidget->item(i, 0)!=nullptr)
            {//есть ячейка
                arr_table[i] = ui->tableWidget->item(i, 0)->text().toInt(&flag);
                if ((flag) and (arr_table[i] >= -100) and (arr_table[i] <= 100))
                    ui->tableWidget->item(i,0)->setBackground(Qt::white);
                else
                {
                    ui->tableWidget->item(i,0)->setBackground(Qt::magenta);
                    error_value = true; //ошибка значения
                }
            }
            else
            {
                ti = new QTableWidgetItem(); //выделили память
                ui->tableWidget->setItem(i,0,ti); //помещаем ячейку в таблицу
                ui->tableWidget->item(i,0)->setBackground(Qt::darkGray); //красим
            }
        }

    if (column != 1) //если ошибка в нескольких столбцах
        QMessageBox::information(this,"Ошибка","Для сортировки необходим 1 столбец", QMessageBox::Ok);
    else if (error_value) //если ошибка в значении
        QMessageBox::information(this,"Ошибка","Таблица содержит неверные данные.",QMessageBox::Ok);
    else if ((ui->radioButton_increasing->isChecked()) and isSorted(arr_table, row, ASC))
        return;
    else if ((ui->radioButton_decreasing->isChecked()) and isSorted(arr_table, row, DESC))
        return;
    else
    {//нет ошибок
        if (ui->spinBox_Row->value() >= 900000)
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Предупреждение", "Данная операция будет выполняться более 5 секунд. \nВы уверены?", QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
                if (ui->radioButton_increasing->isChecked())
                    sort(arr_table, arr_table + row); //быстрая сортировка
                else
                    sort(arr_table, arr_table + row, comp()); //быстрая сортировка по убыванию

                for (int i = 0; i < row; i++) //вывод массива в таблицу
                    ui->tableWidget->item(i,0)->setText(QString::number(arr_table[i]));

                ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
            }
            else
                ui->label_time->setText("Отмена"); //вывод времени операции в мс
        }
        else
        {
            if (ui->radioButton_increasing->isChecked())
                sort(arr_table, arr_table + row); //сортировка быстрая по возрастанию
            else
                sort(arr_table, arr_table + row, comp()); //быстрая сортировка по убыванию

            for (int i = 0; i < row; i++) //вывод массива в таблицу
                ui->tableWidget->item(i,0)->setText(QString::number(arr_table[i]));

            ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
        }
    }

    //чистим память
    arr_table = clearArr(arr_table);

    no_auto_change=true;
}

void MainWindow::sort_bogo() //обезьянья сортировка
{
    timer.start(); //запуск таймер

    no_auto_change = false;
    error_value = false;

    //узнаем значения строк и колонок
    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();

    //создаем динамический массив
    int *arr_table = new int [row]; //массив для данных из таблицы

    QTableWidgetItem * ti; //создали указатель

    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
        {
            if (ui->tableWidget->item(i, 0)!=nullptr)
            {//есть ячейка
                arr_table[i] = ui->tableWidget->item(i, 0)->text().toInt(&flag);
                if ((flag) and (arr_table[i] >= -100) and (arr_table[i] <= 100))
                    ui->tableWidget->item(i,0)->setBackground(Qt::white);
                else
                {
                    ui->tableWidget->item(i,0)->setBackground(Qt::magenta);
                    error_value = true; //ошибка значения
                }
            }
            else
            {
                ti = new QTableWidgetItem(); //выделили память
                ui->tableWidget->setItem(i,0,ti); //помещаем ячейку в таблицу
                ui->tableWidget->item(i,0)->setBackground(Qt::darkGray); //красим
            }
        }

    if (column != 1) //если ошибка в нескольких столбцах
        QMessageBox::information(this,"Ошибка","Для сортировки необходим 1 столбец", QMessageBox::Ok);
    else if (error_value) //если ошибка в значении
        QMessageBox::information(this,"Ошибка","Таблица содержит неверные данные.",QMessageBox::Ok);
    else if ((ui->radioButton_increasing->isChecked()) and isSorted(arr_table, row, ASC))
        return;
    else if ((ui->radioButton_decreasing->isChecked()) and isSorted(arr_table, row, DESC))
        return;
    else
    {//нет ошибок
        if (ui->spinBox_Row->value() >= 10)
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Предупреждение", "Данная операция будет выполняться более 5 секунд. \nВы уверены?", QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes)
            {
                QMessageBox::information(this, "Ошибка", "Ты куда прешь, долго", QMessageBox::Ok);
            }

            ui->label_time->setText("Отмена"); //вывод времени операции в мс
        }
        else
        {
            if (ui->radioButton_increasing->isChecked())
                bogo(arr_table, row, ASC); //сортировка bogo по возрастанию
            else
                bogo(arr_table, row, DESC); //сортировка bogo по убыванию

            for (int i = 0; i < row; i++) //вывод массива в таблицу
                ui->tableWidget->item(i,0)->setText(QString::number(arr_table[i]));

            ui->label_time->setText(QString::number(timer.elapsed()) + "мс"); //вывод времени операции в мс
        }
    }

    //чистим память
    arr_table = clearArr(arr_table);

    no_auto_change=true;
}


//Search


void MainWindow::search_enum() //поиск перебором
{
    no_auto_change = false;
    error_value = false;

    //узнаем значения строк и колонок
    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();

    //создаем динамический массив
    int *arr_table = new int [row]; //массив для данных из таблицы

    QTableWidgetItem * ti; //создали указатель

    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
        {
            if (ui->tableWidget->item(i, 0)!=nullptr)
            {//есть ячейка
                arr_table[i] = ui->tableWidget->item(i, 0)->text().toInt(&flag);
                if ((flag) and (arr_table[i] >= -100) and (arr_table[i] <= 100))
                    ui->tableWidget->item(i,0)->setBackground(Qt::white);
                else
                {
                    ui->tableWidget->item(i,0)->setBackground(Qt::magenta);
                    error_value = true; //ошибка значения
                }
            }
            else
            {
                ti = new QTableWidgetItem(); //выделили память
                ui->tableWidget->setItem(i,0,ti); //помещаем ячейку в таблицу
                ui->tableWidget->item(i,0)->setBackground(Qt::darkGray); //красим
            }
        }

    if (column != 1) //если ошибка в нескольких столбцах
        QMessageBox::information(this,"Ошибка","Для сортировки необходим 1 столбец", QMessageBox::Ok);
    else if (error_value) //если ошибка в значении
        QMessageBox::information(this,"Ошибка","Таблица содержит неверные данные.",QMessageBox::Ok);
    else
    {//нет ошибок
        QString arrStr = ""; //строка для ответа

        int *ans = new int [row]; //создаем массив для ответов

        //переменные для счета
        int key = ui->lineEdit_search->text().toInt(&flag);//искомое значение
        if (!flag) //проверка коюча
        {
            ui->listWidget_ans_search_num->setVisible(false);
            ui->label_search->clear();
            ui->label_ans_search_entry->clear();
            QMessageBox::information(this, "Ошибка", "Некоректное значение ключа", QMessageBox::Ok);
        }
        else if ((key > 100) or (key < -100))
        {
            ui->listWidget_ans_search_num->setVisible(false);
            ui->label_search->clear();
            ui->label_ans_search_entry->clear();
            QMessageBox::information(this, "Ошибка", "Значение ключа от - 100 до 100", QMessageBox::Ok);
        }
        else
        {
            int count_enum = 0, count_ans = 0;

            for (int i=0; i<row; i++)
            {
                count_enum += 1;
                if (arr_table[i] == key)
                    ans[count_ans++] = i;
            }
            //вывод массива в таблицу
            //Без проверки ячеек, так как она уже проведена
            if (count_ans != 0)
            {
                for (int i = 0; i < count_ans; i++)
                {
                    arrStr += QString::number(ans[i] + 1) + " ";
                    ui->tableWidget->item(ans[i], 0)->setBackground(Qt::green);
                }
                ui->listWidget_ans_search_num->addItem(arrStr); // вывод строчек
                ui->label_ans_search_entry->setNum(count_enum); // вывод кол-во вхождений
            }
            else
            {
                ui->listWidget_ans_search_num->addItem("Искомого значения нет"); // вывод значения null
                ui->label_ans_search_entry->setNum(count_enum);
            }

            ans = clearArr(ans); //чистка памяти

            ui->listWidget_ans_search_num->setVisible(true);
            ui->label_search->setText("Поиск перебором");

            arr_table = clearArr(arr_table);
        }
    }

    //чистим память
    arr_table = clearArr(arr_table);


    no_auto_change=true;
}

void MainWindow::search_binary() //бинарный поиск
{
    no_auto_change = false;
    error_value = false;

    //узнаем значения строк и колонок
    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();

    //создаем динамический массив
    int *arr_table = new int [row]; //массив для данных из таблицы

    QTableWidgetItem * ti; //создали указатель

    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
        {
            if (ui->tableWidget->item(i, 0)!=nullptr)
            {//есть ячейка
                arr_table[i] = ui->tableWidget->item(i, 0)->text().toInt(&flag);
                if ((flag) and (arr_table[i] >= -100) and (arr_table[i] <= 100))
                    ui->tableWidget->item(i,0)->setBackground(Qt::white);
                else
                {
                    ui->tableWidget->item(i,0)->setBackground(Qt::magenta);
                    error_value = true; //ошибка значения
                }
            }
            else
            {
                ti = new QTableWidgetItem(); //выделили память
                ui->tableWidget->setItem(i,0,ti); //помещаем ячейку в таблицу
                ui->tableWidget->item(i,0)->setBackground(Qt::darkGray); //красим
            }
        }

    if (column != 1) //если ошибка в нескольких столбцах
        QMessageBox::information(this,"Ошибка","Для сортировки необходим 1 столбец", QMessageBox::Ok);
    else if (error_value) //если ошибка в значении
        QMessageBox::information(this,"Ошибка","Таблица содержит неверные данные.",QMessageBox::Ok);
    else
    {//нет ошибок
        QString arrStr = ""; //строка для ответа

        //переменные для счета
        int key = ui->lineEdit_search->text().toInt(&flag);//искомое значение
        if (!flag)
        {
            ui->listWidget_ans_search_num->setVisible(false);
            ui->label_search->clear();
            ui->label_ans_search_entry->clear();
            QMessageBox::information(this, "Ошибка", "Некоректное значение ключа", QMessageBox::Ok);
        }
        else if ((key > 100) or (key < -100))
        {
            ui->listWidget_ans_search_num->setVisible(false);
            ui->label_search->clear();
            ui->label_ans_search_entry->clear();
            QMessageBox::information(this, "Ошибка", "Значение ключа от - 100 до 100", QMessageBox::Ok);
        }
        else
        {
            int count_enum = 0, mid = 0, left = 0, right = row;
            flag = false;
            //поиск бинарным способом
            while ((left <= right) and (flag != true))
            {
                count_enum += 1;
                mid = (right + left) / 2;
                if (arr_table[mid] == key)
                    flag = true;
                if (arr_table[mid] > key)
                    right = mid - 1;
                else
                    left = mid + 1;
            }

            if (flag)
            {//если значения найдены
                int step = 1;

                while (arr_table[mid] == arr_table[mid - step]) //поиск всех значений
                {
                    step++;
                }
                left = mid - step + 1; //нашли самый левый элемент в массиве
                step= 1;
                arrStr += QString::number(left + 1) + " ";
                ui->tableWidget->item(left, 0)->setBackground(Qt::green);
                while (arr_table[left] == arr_table[left + step]) //сравниваем самый левый элемент с последующими
                {
                    arrStr += QString::number(left + step + 1) + " ";
                    ui->tableWidget->item(left + step, 0)->setBackground(Qt::green);
                    step++;
                    count_enum += 1;
                }
                ui->listWidget_ans_search_num->addItem(arrStr);
                ui->label_ans_search_entry->setNum(count_enum);
            }
            else
            {//если значения ненайдены
                ui->listWidget_ans_search_num->addItem("Искомого значения нет");
                ui->label_ans_search_entry->setNum(count_enum);
            }
            ui->listWidget_ans_search_num->setVisible(true);
            ui->label_search->setText("Бинарный поиск");

        }
    }

    //чистим память
    arr_table = clearArr(arr_table);

    no_auto_change=true;
}


//Remove duplicate


void MainWindow::remove_duplicate() //удаление дубликатов с помощью двух массивов
{
    no_auto_change = false;
    error_value = false;

    int row = ui->tableWidget->rowCount();
    int column = ui->tableWidget->columnCount();

    int *arr_table = new int [row];

    QTableWidgetItem * ti; //создали указатель

    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
        {
            if (ui->tableWidget->item(i, 0)!=nullptr)
            {//есть ячейка
                arr_table[i] = ui->tableWidget->item(i, 0)->text().toInt(&flag);
                if ((flag) and (arr_table[i] >= -100) and (arr_table[i] <= 100))
                    ui->tableWidget->item(i,0)->setBackground(Qt::white);
                else
                {
                    ui->tableWidget->item(i,0)->setBackground(Qt::magenta);
                    error_value = true; //ошибка значения
                }
            }
            else
            {
                ti = new QTableWidgetItem(); //выделили память
                ui->tableWidget->setItem(i,0,ti); //помещаем ячейку в таблицу
                ui->tableWidget->item(i,0)->setBackground(Qt::darkGray); //красим
            }
        }

    if (column != 1) //если ошибка в нескольких столбцах
        QMessageBox::information(this,"Ошибка","Для сортировки необходим 1 столбец", QMessageBox::Ok);
    else if (error_value) //если ошибка в значении
        QMessageBox::information(this,"Ошибка","Таблица содержит неверные данные.",QMessageBox::Ok);
    else
    {//нет ошибок
        //удаление дубликатов
        int count = 0; //счетчик количества уникальных значений
        int max = 201; //возможных значений

        int *arr_ans = new int [row]; //массив с уникальными значениями
        int arr_temp[201]; //массив всех возможных значений в таблице

        for (int i = 0; i < max; i++) //заполняем временный массив
            arr_temp[i] = 0;

        for (int i = 0; i < row; i++) //сравниваем
        {
            ui->tableWidget->item(i,0)->setText(""); //чистим таблицу от старых значений

            if (!arr_temp[arr_table[i] + 100]) //если значение уникально (+100, т.к. у нас есть значения в массиве -100, чтобы не было непоняток)
            {
                arr_ans[count++] = arr_table[i]; //... то сохранить его в результ массив
                arr_temp[arr_table[i] + 100] = 1; //пометить значение как использованное
            }
        }

        for (int i = 0; i < count; i++) //вывод всех значений
            ui->tableWidget->item(i, 0)->setText(QString::number(arr_ans[i]));

        ui->tableWidget->setRowCount(count); //изменение количества строк на нужное
        ui->spinBox_Row->setValue(count); //изменили спинбокс под значения строк

        arr_ans = clearArr(arr_ans);
    }

    arr_table = clearArr(arr_table);


    no_auto_change = true;

}


//Open arr


void MainWindow::on_pushButton_openTxt_clicked() //open txt
{
    no_auto_change = false;

    ui->label_time->clear();

    if (ui->tableWidget->columnCount() != 1)
    {//если не 1 стобец
        //Выводим ошибку
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Чтобы считать массив из файла txt необходим 1 столбец. \nИзменить количество столбцов?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int res = msgBox.exec();

        //Обработка ошибки
        switch (res)
            {//какую кнопку нажал юзер
            case QMessageBox::Yes:
            {//нажали кнопку Yes
                ui->tableWidget->setColumnCount(1); //изменили кол-во столбцов на 1
                on_pushButton_openTxt_clicked(); //запустили программу опять
                break;
            }
            case QMessageBox::No:
            {//нажали кнопку No
                break; //ничего не делаем
            }
        }
    }
    else
    {//если 1 стобец
        openTxt();
    }
    no_auto_change = true;
}

void MainWindow::on_pushButton_openBin_clicked() //open bin
{
    no_auto_change = false;

    ui->label_time->clear();

    if (ui->tableWidget->columnCount() != 1)
    {//если не 1 стобец
        //Выводим ошибку
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Чтобы считать массив из файла bin необходим 1 столбец. \nИзменить количество столбцов?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int res = msgBox.exec();

        //Обработка ошибки
        switch (res)
            {//какую кнопку нажал юзер
            case QMessageBox::Yes:
            {//нажали кнопку Yes
                ui->tableWidget->setColumnCount(1); //изменили кол-во столбцов на 1
                on_pushButton_openBin_clicked(); //запустили программу опять
                break;
            }
            case QMessageBox::No:
            {//нажали кнопку No
                break; //ничего не делаем
            }
        }
    }
    else
    {//если 1 стобец
        openBin();
    }
    no_auto_change = true;
}


//Save arr


void MainWindow::on_pushButton_saveTxt_clicked() //save as txt
{
    no_auto_change = false;

    ui->label_time->clear();

    if (ui->tableWidget->columnCount() != 1)
    {//если не 1 стобец
        //Выводим ошибку
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Чтобы сохранить массив в файл txt необходим 1 столбец. \nИзменить количество столбцов?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int res = msgBox.exec();

        //Обработка ошибки
        switch (res)
            {//какую кнопку нажал юзер
            case QMessageBox::Yes:
            {//нажали кнопку Yes
                ui->tableWidget->setColumnCount(1); //изменили кол-во столбцов на 1
                on_pushButton_saveTxt_clicked(); //return clicked saveTxt
                break;
            }
            case QMessageBox::No:
            {//нажали кнопку No
                break; //ничего не делаем
            }
        }
    }
    else
    {//если 1 стобец
        flag = true;

        for (int i = 0; i < ui->spinBox_Row->value(); i++)
        {
            if (ui->tableWidget->item(i,0) == nullptr or ui->tableWidget->item(i,0)->text().isEmpty())
            {//если не существует ячейки или пустая строка
                QMessageBox msgBox;
                msgBox.setWindowTitle("Ошибка");
                msgBox.setText("В массиве есть пустые строки. \nВы хотите продолжить?");
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                int res = msgBox.exec();

                //Обработка ошибки
                switch (res)
                {//какую кнопку нажал юзер
                    case QMessageBox::Yes:
                    {//нажали кнопку Yes
                        flag = false;
                        saveTxt();
                        break;
                    }
                    case QMessageBox::No:
                    {//нажали кнопку No
                        flag = false;
                        break; //ничего не делаем
                    }
                }
                break;
            }
        }

        if (flag == true)
        {//если нет пустых значений
            saveTxt();
        }
    }

    no_auto_change = true;
}

void MainWindow::on_pushButton_saveBin_clicked() //save as bin
{
    no_auto_change = false;

    ui->label_time->clear();

    if (ui->tableWidget->columnCount() != 1)
    {//если не 1 стобец
        //Выводим ошибку
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Чтобы сохранить массив в файл txt необходим 1 столбец. \nИзменить количество столбцов?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int res = msgBox.exec();

        //Обработка ошибки
        switch (res)
            {//какую кнопку нажал юзер
            case QMessageBox::Yes:
            {//нажали кнопку Yes
                ui->tableWidget->setColumnCount(1); //изменили кол-во столбцов на 1
                on_pushButton_saveBin_clicked(); //return clicked saveTxt
                break;
            }
            case QMessageBox::No:
            {//нажали кнопку No
                break; //ничего не делаем
            }
        }
    }
    else
    {//если 1 стобец
        flag = true;

        for (int i = 0; i < ui->spinBox_Row->value(); i++)
        {
            ui->tableWidget->item(i,0)->text().toInt(&flag);
            if (ui->tableWidget->item(i,0) == nullptr or ui->tableWidget->item(i,0)->text().isEmpty() or !flag)
            {//если не существует ячейки или пустая строка или значение некоректно
                QMessageBox msgBox;
                msgBox.setWindowTitle("Ошибка");
                msgBox.setText("В массиве есть неверные значения. \nМассив будет сохранен некорректно \nВы хотите продолжить?");
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                int res = msgBox.exec();

                //Обработка ошибки
                switch (res)
                {//какую кнопку нажал юзер
                    case QMessageBox::Yes:
                    {//нажали кнопку Yes
                        flag = false;
                        saveBin();
                        break;
                    }
                    case QMessageBox::No:
                    {//нажали кнопку No
                        flag = false;
                        break; //ничего не делаем
                    }
                }
                break;
            }
        }

        if (flag)
        {//если нет пустых значений
            saveBin();
        }
    }

    no_auto_change = true;
}


//function for open/save txt/bin


void MainWindow::saveTxt() //func save as txt
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить как "), "C:/Users/sulle/OneDrive/Документы", tr("Text files (*.txt)"));

    if (fileName.isEmpty())
    {
        QMessageBox::information(this, "Ошибка", "Файл не выбран");
    }
    else
    {
        //Open file
        QFile file;
        file.setFileName(fileName);
        file.open(QIODevice::WriteOnly);

        //Переменные для операции
        int size = ui->spinBox_Row->value(); //размер массива
        QString str; //строка для записи массива в файл txt
        str.setNum(size); //write size in 1 string
        str.append("\n"); //append enter
        file.write(str.toUtf8());

        //Записываем значения в txt документ
        for (int i = 0; i < size; i++)
        {
            if (ui->tableWidget->item(i,0)==nullptr)
            {//если ячейка не существует, то создаем ее
                QTableWidgetItem *ti;
                ti = new QTableWidgetItem;
                ui->tableWidget->setItem(i,0,ti);
            }

            //Write arg if txt document
            str = ui->tableWidget->item(i,0)->text();
            str.append("\n");
            file.write(str.toUtf8());

        }
        file.close();
    }
}

void MainWindow::openTxt() //func open as txt
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть txt"), "C:/Users/sulle/OneDrive/Документы", tr("Text files (*.txt)"));

    if (fileName.isEmpty())
    {
        QMessageBox::information(this, "Ошибка", "Файл не выбран");
    }
    else
    {
        //Open file
        QFile file;
        file.setFileName(fileName);
        file.open(QIODevice::ReadOnly);


        //arg
        int size;
        QString str;
        QByteArray ba; //pointer in txt
        bool flag;


        //find the size
        ba = file.readLine(); //pointer on the 1 line
        str.clear();
        str.append(ba);
        str.remove("\n");

        size = str.toInt(&flag); //size file

        if (!flag)
        {
            QMessageBox::information(this, "Ошибка", "Не могу считать");
        }
        else
        {
            ui->spinBox_Row->setValue(size);

            for (int i = 0; i < size; i++)
            {

                if (ui->tableWidget->item(i,0)==nullptr)
                {//cell not exist
                    QTableWidgetItem *ti;
                    ti = new QTableWidgetItem;
                    ui->tableWidget->setItem(i,0,ti);
                }
                else if (ui->tableWidget->item(i,0)->text().isEmpty())
                {//cell empty
                    ui->tableWidget->item(i,0)->setBackground(Qt::red); //changed background of empty cells
                }

                //write arg in arr tableWidjet
                ba = file.readLine();
                str.clear();
                str.append(ba);
                str.remove("\n");

                int tmp = str.toInt(&flag); //check the cell for int
                if (!flag)
                    ui->tableWidget->item(i,0)->setBackground(Qt::red); //changed background of non int arg
                else
                    if ((tmp >= -100) and (tmp <= 100))
                        ui->tableWidget->item(i,0)->setBackground(Qt::white); //changed background of -100<=cells<=100
                    else
                        ui->tableWidget->item(i,0)->setBackground(Qt::magenta);

                ui->tableWidget->item(i,0)->setText(str);
            }
        }
        file.close();
    }
}

void MainWindow::saveBin() //func save as bin
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить как bin"), "C:/Users/sulle/OneDrive/Документы", tr("Binary files (*.bin)"));

    if (fileName.isEmpty())
    {
        QMessageBox::information(this, "Ошибка", "Файл не выбран");
    }
    else
    {
        //Open file
        QFile file;
        file.setFileName(fileName);
        file.open(QIODevice::WriteOnly);

        //Create arg
        char *mas;
        int size_int = sizeof(int);
        size_t size_t = sizeof(int);
        mas = new char[size_int];

        int size = ui->spinBox_Row->value(); //size arr

        memcpy(mas, &size, size_t);
        file.write(mas, size_int);

        for (int i = 0; i < size; i++)
        {
            if (ui->tableWidget->item(i,0)==nullptr)
            {//cell not exist
                QTableWidgetItem *ti;
                ti = new QTableWidgetItem;
                ui->tableWidget->setItem(i,0,ti);
            }

            int tmp = ui->tableWidget->item(i,0)->text().toInt(&error);

            if (!(error and  (tmp >= -100 and tmp <= 100)))//check bad arg
                tmp = 101;

            memcpy(mas, &tmp, size_t);
            file.write(mas, size_int);
        }
        file.close();
        delete [] mas;
        mas = nullptr;
    }
}

void MainWindow::openBin() //fucn open as bin
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть как bin"), "C:/Users/sulle/OneDrive/Документы", tr("Binary files (*.bin)"));

    if (fileName.isEmpty())
    {//check empty fileName
        QMessageBox::information(this, "Ошибка", "Файл не выбран");
    }
    else
    {
        //Open file
        QFile file;
        file.setFileName(fileName);
        file.open(QIODevice::ReadOnly);

        //Create arg
        char *mas;
        int size_int = sizeof(int);
        size_t size_t = sizeof(int);
        mas = new char[size_int];

        int size = ui->spinBox_Row->value(); //size table (row)

        //Read size arr in .bin
        file.read(mas, size_int);
        memcpy(&size, mas, size_t);

        if (file.size() != size_int*(size + 1)) //check file
            QMessageBox::information(this, "Ошибка", "Файл не выбран");
        else
        {//all good
            ui->spinBox_Row->setValue(size);

            for (int i = 0; i < size; i++)
            {
                if (ui->tableWidget->item(i,0)==nullptr)
                {
                    QTableWidgetItem *ti;
                    ti = new QTableWidgetItem;
                    ui->tableWidget->setItem(i,0,ti);
                }

                //Write arg in table
                int tmp;
                file.read(mas, size_int);
                memcpy(&tmp, mas, size_t);
                ui->tableWidget->item(i,0)->setText(QString::number(tmp));

                //Check -100<=x<=100
                if ((tmp >= -100) and (tmp <= 100))
                    ui->tableWidget->item(i,0)->setBackground(Qt::white); //changed background of -100<=cells<=100
                else if (tmp == 101)
                {
                    ui->tableWidget->item(i,0)->setText("error");
                    ui->tableWidget->item(i,0)->setBackground(Qt::red);
                }
            }
            file.close();
            delete [] mas;
            mas = nullptr;
        }
    }
}
