#include "person.h"
#include "ui_person.h"

#include <QtSql/qsqltablemodel.h>
#include<QAbstractItemView>
#include "qabstractitemview.h"
#include <QSqlRelationalTableModel>
#include "QList.h"
#include<QModelIndexList>
#include <QtGui/qevent.h>
#include<QDateEdit>
#include<QGroupBox>
#include<QVBoxLayout>
#include<QDate>
#include<QItemDelegate>
#include<QDoubleSpinBox>
#include<QItemDelegate>
#include<QPushButton>

Person::Person(QWidget *parent):
    QWidget(parent),
    ui (new Ui::Person)
{
    ui->setupUi(this);
    setWindowTitle(Person::tr("Справочник-Граждане")); //ПОДУМАТЬ!!!!!!
}

Person::~Person()
{
    delete ui;
}

QSqlTableModel model_1;

void Person::SetTable(QString tableName, QString filter, QList<QString> list )
{
	//------------растягиваем колонки по ширине содержимого
	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // Разрешаем выделение строк
	ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);// Устанавливаем режим выделения лишь одно строки в таблице
	// ------Устанавливаем размер колонок по содержимому
	ui->tableView->resizeColumnsToContents();
	ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
	//ui->tableView->setEditTriggers(QAbstractItemView::OnFieldChange); //блокируем редактирование ячейки
	ui->tableView->horizontalHeader()->setStretchLastSection(true);
	
	model_1.setTable(tableName);
	tableName=="tb_person";
	ui->tableView->setModel(&model_1);
    while(model_1.canFetchMore())
		model_1.fetchMore();
		setFocus();
          
	model_1.setRelation(model_1.fieldIndex("municipality"),QSqlRelation("tb_municipality","id","municipality"));
    ui->tableView->setColumnHidden(0, true);//скрываем первый столбец
    model_1.setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    model_1.setHeaderData(1, Qt::Horizontal, QObject::tr("ФИО*"));
    model_1.setHeaderData(2, Qt::Horizontal, QObject::tr("Место работы"));
    model_1.setHeaderData(3, Qt::Horizontal, QObject::tr("Должность"));
    model_1.setHeaderData(4, Qt::Horizontal, QObject::tr("Муниципальное образование*"));
    model_1.setHeaderData(5, Qt::Horizontal, QObject::tr("Доп. сведения"));

    //------ширина_столбцов
    ui->tableView->horizontalHeader()->resizeSection(1,300);
     ui->tableView->horizontalHeader()->resizeSection(2,450);
      ui->tableView->horizontalHeader()->resizeSection(3,450);
       ui->tableView->horizontalHeader()->resizeSection(4,200);
        ui->tableView->horizontalHeader()->resizeSection(5,100);
    
	//---подключение к ячейке таблице модуля КАЛЕНДАРЬ
    /*
    MyDEDelegate *ded = new MyDEDelegate(
    true, this);
    ui->tableView->setItemDelegateForColumn(2,ded);
    */
    model_1.select();
}

//Кнопка "Cохраняем_запись"
void Person::on_pushButton_clicked()
{
	model_1.submitAll();
}
// Кнопка "Добавить_строку"
void Person::on_pushButton_2_clicked()
{
	model_1.select() ;
	QModelIndex insertIndex ;
	int row =insertIndex.row() ==-1?0:insertIndex.row();
	model_1.insertRow(row);
	insertIndex=model_1.index(row,0);
}
//кнопка "Удалить запись, начиная с последней"
void Person::on_pushButton_3_clicked()
{
	QModelIndex currentIndex=ui->tableView->selectionModel()->currentIndex();
	model_1.removeRow(currentIndex.row());
}
//кнопка "Отменить последние действие"
void Person::on_pushButton_4_clicked()
{
	model_1.revertAll(); //отмена действия с записью в БД
	model_1.select() ;
}
//Кнопка "Фильтр по ФИО"
void Person::on_pushButton_5_clicked()   
{
	model_1.setFilter("Family LIKE '" + ui->lineEdit->text() + "%'" );
}
//Кнопка "Фильтр по Муниципалитетам"
void Person::on_pushButton_7_clicked() 
{
	QSqlQuery query;
	QString sr_S = ui->lineEdit_2->text();

    query.exec("SELECT t1.id FROM tb_person t2 \
				INNER JOIN tb_municipality t1 ON t1.id=t2.municipality \
				WHERE t1.municipality LIKE '"+sr_S+"%' AND '"+sr_S+"'!='' ");
    if(!query.isActive())             //обработка ошибок
    {
		//ошибка
		qDebug()<<query.lastError().text();
    }

    query.first();

	int count = query.value(0).toInt();		//сохр. результ. SQL запроса в переменную count
	QString str = QString::number(count); 	//переводим стороку из int в str
	model_1.setFilter("tb_person.municipality LIKE '"+str+"%'");
}

//Кнопка "Очистка всех фильров"
void Person::on_pushButton_6_clicked() 
{
	ui->lineEdit->clear();
	ui->lineEdit_2->clear();
	model_1.setFilter("");
}

//Модуль_КАЛЕНДАРЬ
MyView::MyView(QWidget *parent)
       : QTableView(parent) {

        MyDSBDelegate *dsbd = new MyDSBDelegate(
               0.0, 999999.99, 0.05, 2, this);
          setItemDelegateForColumn(3, dsbd);
 }
//------------------------------------------------
	MyDSBDelegate::MyDSBDelegate(
                double min,
                double max,
                double step,
                int precision,
                QObject *parent)
            :QItemDelegate(parent),
            m_min(min),
            m_max(max),
            m_step(step),
            m_precision(precision){
		}

    QWidget *MyDSBDelegate::createEditor(
                QWidget *parent,
                const QStyleOptionViewItem& /* option */,
                const QModelIndex& /* index */) const {
			QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
			editor->setMinimum(m_min);
			editor->setMaximum(m_max);
			editor->setDecimals(m_precision);
			editor->setSingleStep(m_step);
			editor->installEventFilter(const_cast<MyDSBDelegate*>(this));
			return editor;
		}

		void MyDSBDelegate::setEditorData(
					QWidget *editor,
					const QModelIndex &index) const {
				double value = index.model()->data(
				index, Qt::EditRole).toDouble();
				QDoubleSpinBox *dsb = static_cast<QDoubleSpinBox*>(editor);
				dsb->setValue(value);
		}
	
		void MyDSBDelegate::setModelData(
					QWidget *editor,
					QAbstractItemModel *model,
					const QModelIndex& index) const {
				QDoubleSpinBox *dsb = static_cast<QDoubleSpinBox*>(editor);
				dsb->interpretText();
				double value = dsb->value();
				model->setData(index, value);
		}
	
		void MyDSBDelegate::updateEditorGeometry(
					QWidget *editor,
					const QStyleOptionViewItem &option,
					const QModelIndex& /* index */) const {
				editor->setGeometry(option.rect);
		}

//-------------------------
    MyDEDelegate::MyDEDelegate(
                bool calpopup,
                QObject *parent)
            : QItemDelegate(parent),
            m_calpopup(calpopup) {
		}

    QWidget *MyDEDelegate::createEditor(
                QWidget *parent,
                const QStyleOptionViewItem& /* option */,
                const QModelIndex& /* index */) const {
			QDateEdit *editor = new QDateEdit(parent);
			editor->setCalendarPopup(m_calpopup);
			editor->installEventFilter(const_cast<MyDEDelegate*>(this));
			return editor;
		}

		void MyDEDelegate::setEditorData(
					QWidget *editor,
					const QModelIndex &index) const {
				QDate value = index.model()->data(
				index, Qt::EditRole).toDate();
				QDateEdit *de = static_cast<QDateEdit*>(editor);
				de->setDate(value);
		}

		void MyDEDelegate::setModelData(
					QWidget *editor,
					QAbstractItemModel *model,
					const QModelIndex& index) const {
				QDateEdit *de = static_cast<QDateEdit*>(editor);
				de->interpretText();
				QDate value = de->date();
				model->setData(index, value);
		}

		void MyDEDelegate::updateEditorGeometry(
					QWidget *editor,
					const QStyleOptionViewItem &option,
					const QModelIndex& /* index */) const {
				editor->setGeometry(option.rect);
	  }
