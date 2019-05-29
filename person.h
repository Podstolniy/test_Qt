#ifndef PERSON_H
#define PERSON_H

#include <QWidget>
#include <QFileInfo>
#include <QSqlTableModel>
#include <QTableView>
#include <QMessageBox>
#include <QtSql>
#include <QAbstractItemModel>
#include <QtSql/qsqltablemodel.h>
#include <QDataWidgetMapper>
#include<QAbstractItemView>
#include "qabstractitemview.h"
#include<QLabel>


class QDataWidgetMapper;
class QPushButton;
class QItemDelegate;
class QSqlRelationalTableModel;
class QAbstractItemView;
class QAbstractItemViewPrivate;

namespace Ui {
     class Person;
}

class Person :  public QWidget
{
    Q_OBJECT
public:
    explicit Person(QWidget *parent = 0);
    ~Person();


    //QTableView view;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    	bool setData(const QModelIndex &index, const QVariant &value, int role);

    QString curFile;
	void SetTable(QString tableName, QString filter, QList<QString> list);
	void setModel(QAbstractItemModel *table);

signals:
//void firstWindow();  // Сигнал для первого окна на открытие

public slots:
void on_pushButton_clicked();   // Кнопка "Cохраняем_запись"
void on_pushButton_2_clicked(); // Кнопка "Добавить_строку"
void on_pushButton_3_clicked(); // Кнопка "Удалить_запись"
void on_pushButton_4_clicked(); // Кнопка "Отменить_действие"
void on_pushButton_5_clicked(); // Кнопка "Фильтр_по_ФИО"
void on_pushButton_6_clicked(); // Кнопка "Очистка_всех_фильтров"
void on_pushButton_7_clicked(); // Кнопка "Фильтр_по_Муниципальным_образованиям"

private:
    QString strippedName(const QString &fullFileName);
    Ui::Person *ui;
    bool isUntitled;
    QSqlRelationalTableModel model_1;

};

//---------------------------
class MyView : public QTableView {
	Q_OBJECT
 public:
    MyView(QWidget *parent = 0);
 private:
    //virtual void resizeEvent(QResizeEvent *event);
};

//--------------------------------------------
    class MyDSBDelegate : public QItemDelegate {
		Q_OBJECT
    public:
		MyDSBDelegate(double min=0.00,
			double max=999999999.99,
			double step=0.1,
                    	int precision=2,
                QObject *parent = 0);
			QWidget *createEditor(
			QWidget *parent,
			const QStyleOptionViewItem &option,
                    const QModelIndex &index) const;
        void setEditorData(QWidget *editor,
                const QModelIndex &index) const;
        void setModelData(QWidget *editor,
                QAbstractItemModel *model,
			const QModelIndex &index) const;
        void updateEditorGeometry(
                QWidget *editor,
			const QStyleOptionViewItem &option,
			const QModelIndex &index) const;
    private:
        double m_min;
        double m_max;
        double m_step;
        int m_precision;
};

//-----------Класс_для_календаря
    class MyDEDelegate : public QItemDelegate {
		Q_OBJECT
    public:
        MyDEDelegate(bool calpopup = true,
	QObject *parent = 0);
        QWidget *createEditor(
                QWidget *parent,
			const QStyleOptionViewItem &option,
			const QModelIndex &index) const;


        void setItemDelegateForColumn(int column, QAbstractItemDelegate *delegate);
		QAbstractItemDelegate *itemDelegateForColumn(int column) const;

        void setEditorData(QWidget *editor,
                	const QModelIndex &index) const;
        void setModelData(QWidget *editor,
                QAbstractItemModel *model,
			const QModelIndex &index) const;
        void updateEditorGeometry(
		QWidget *editor,
			const QStyleOptionViewItem &option,
			const QModelIndex &index) const;
    private:
        bool m_calpopup;
};

#endif // PERSON_H
