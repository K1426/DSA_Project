/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *searchPage;
    QTextBrowser *resultsBrowser;
    QLineEdit *lineEdit;
    QLabel *labelEnter;
    QPushButton *buttonSearch;
    QPushButton *buttonCloseSearch;
    QLabel *labelName;
    QPushButton *buttonGoAdd;
    QWidget *resultPage;
    QTextEdit *resultsEdit;
    QPushButton *buttonGoBack;
    QPushButton *buttonCloseResult;
    QWidget *addPage;
    QPushButton *buttonCloseAdd;
    QPushButton *buttonGoSearch;
    QLineEdit *editPath;
    QLabel *labelPath;
    QLabel *labelSuccess;
    QPushButton *buttonPath;
    QStatusBar *statusbar;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(960, 540);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(52, 108, 212);"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, 0, 960, 540));
        searchPage = new QWidget();
        searchPage->setObjectName("searchPage");
        resultsBrowser = new QTextBrowser(searchPage);
        resultsBrowser->setObjectName("resultsBrowser");
        resultsBrowser->setGeometry(QRect(230, 150, 600, 300));
        QFont font;
        font.setFamilies({QString::fromUtf8("Times New Roman")});
        font.setPointSize(12);
        resultsBrowser->setFont(font);
        resultsBrowser->setStyleSheet(QString::fromUtf8("color: rgb(52, 108, 212);\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius: 20px;"));
        lineEdit = new QLineEdit(searchPage);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(230, 100, 500, 30));
        lineEdit->setFont(font);
        lineEdit->setStyleSheet(QString::fromUtf8("color: rgb(52, 108, 212);\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius: 15px;"));
        labelEnter = new QLabel(searchPage);
        labelEnter->setObjectName("labelEnter");
        labelEnter->setGeometry(QRect(160, 100, 50, 30));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Times New Roman")});
        font1.setPointSize(12);
        font1.setBold(true);
        labelEnter->setFont(font1);
        labelEnter->setStyleSheet(QString::fromUtf8("color: rgb(175, 204, 255);"));
        buttonSearch = new QPushButton(searchPage);
        buttonSearch->setObjectName("buttonSearch");
        buttonSearch->setGeometry(QRect(750, 100, 80, 30));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Times New Roman")});
        font2.setPointSize(10);
        font2.setBold(true);
        buttonSearch->setFont(font2);
        buttonSearch->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius: 10px;\n"
"	color: rgb(52, 108, 212);\n"
"	background-color: rgb(175, 204, 255);\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"	color: rgb(40, 82, 160);\n"
"	background-color: rgb(155, 192, 255);\n"
"}"));
        buttonCloseSearch = new QPushButton(searchPage);
        buttonCloseSearch->setObjectName("buttonCloseSearch");
        buttonCloseSearch->setGeometry(QRect(30, 30, 100, 30));
        buttonCloseSearch->setFont(font2);
        buttonCloseSearch->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius: 10px;\n"
"	color: rgb(52, 108, 212);\n"
"	background-color: rgb(175, 204, 255);\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"	color: rgb(40, 82, 160);\n"
"	background-color: rgb(155, 192, 255);\n"
"}"));
        labelName = new QLabel(searchPage);
        labelName->setObjectName("labelName");
        labelName->setGeometry(QRect(230, 50, 500, 30));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Times New Roman")});
        font3.setPointSize(20);
        font3.setBold(true);
        labelName->setFont(font3);
        labelName->setStyleSheet(QString::fromUtf8("color: rgb(175, 204, 255);"));
        labelName->setAlignment(Qt::AlignmentFlag::AlignCenter);
        buttonGoAdd = new QPushButton(searchPage);
        buttonGoAdd->setObjectName("buttonGoAdd");
        buttonGoAdd->setGeometry(QRect(30, 80, 100, 30));
        buttonGoAdd->setFont(font2);
        buttonGoAdd->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius: 10px;\n"
"	color: rgb(52, 108, 212);\n"
"	background-color: rgb(175, 204, 255);\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"	color: rgb(40, 82, 160);\n"
"	background-color: rgb(155, 192, 255);\n"
"}"));
        stackedWidget->addWidget(searchPage);
        resultPage = new QWidget();
        resultPage->setObjectName("resultPage");
        resultsEdit = new QTextEdit(resultPage);
        resultsEdit->setObjectName("resultsEdit");
        resultsEdit->setGeometry(QRect(180, 50, 700, 400));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Times New Roman")});
        font4.setPointSize(16);
        resultsEdit->setFont(font4);
        resultsEdit->setStyleSheet(QString::fromUtf8("color: rgb(52, 108, 212);\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius: 20px;"));
        buttonGoBack = new QPushButton(resultPage);
        buttonGoBack->setObjectName("buttonGoBack");
        buttonGoBack->setGeometry(QRect(30, 80, 100, 30));
        buttonGoBack->setFont(font2);
        buttonGoBack->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius: 10px;\n"
"	color: rgb(52, 108, 212);\n"
"	background-color: rgb(175, 204, 255);\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"	color: rgb(40, 82, 160);\n"
"	background-color: rgb(155, 192, 255);\n"
"}"));
        buttonCloseResult = new QPushButton(resultPage);
        buttonCloseResult->setObjectName("buttonCloseResult");
        buttonCloseResult->setGeometry(QRect(30, 30, 100, 30));
        buttonCloseResult->setFont(font2);
        buttonCloseResult->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius: 10px;\n"
"	color: rgb(52, 108, 212);\n"
"	background-color: rgb(175, 204, 255);\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"	color: rgb(40, 82, 160);\n"
"	background-color: rgb(155, 192, 255);\n"
"}"));
        stackedWidget->addWidget(resultPage);
        addPage = new QWidget();
        addPage->setObjectName("addPage");
        buttonCloseAdd = new QPushButton(addPage);
        buttonCloseAdd->setObjectName("buttonCloseAdd");
        buttonCloseAdd->setGeometry(QRect(30, 30, 100, 30));
        buttonCloseAdd->setFont(font2);
        buttonCloseAdd->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius: 10px;\n"
"	color: rgb(52, 108, 212);\n"
"	background-color: rgb(175, 204, 255);\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"	color: rgb(40, 82, 160);\n"
"	background-color: rgb(155, 192, 255);\n"
"}"));
        buttonGoSearch = new QPushButton(addPage);
        buttonGoSearch->setObjectName("buttonGoSearch");
        buttonGoSearch->setGeometry(QRect(30, 80, 100, 30));
        buttonGoSearch->setFont(font2);
        buttonGoSearch->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius: 10px;\n"
"	color: rgb(52, 108, 212);\n"
"	background-color: rgb(175, 204, 255);\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"	color: rgb(40, 82, 160);\n"
"	background-color: rgb(155, 192, 255);\n"
"}"));
        editPath = new QLineEdit(addPage);
        editPath->setObjectName("editPath");
        editPath->setGeometry(QRect(230, 180, 500, 30));
        editPath->setFont(font);
        editPath->setStyleSheet(QString::fromUtf8("color: rgb(52, 108, 212);\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius: 15px;"));
        labelPath = new QLabel(addPage);
        labelPath->setObjectName("labelPath");
        labelPath->setGeometry(QRect(230, 120, 311, 30));
        QFont font5;
        font5.setFamilies({QString::fromUtf8("Times New Roman")});
        font5.setPointSize(14);
        font5.setBold(true);
        labelPath->setFont(font5);
        labelPath->setStyleSheet(QString::fromUtf8("color: rgb(175, 204, 255);"));
        labelSuccess = new QLabel(addPage);
        labelSuccess->setObjectName("labelSuccess");
        labelSuccess->setGeometry(QRect(330, 270, 300, 30));
        labelSuccess->setFont(font5);
        labelSuccess->setStyleSheet(QString::fromUtf8("color: rgb(175, 204, 255);"));
        labelSuccess->setAlignment(Qt::AlignmentFlag::AlignCenter);
        buttonPath = new QPushButton(addPage);
        buttonPath->setObjectName("buttonPath");
        buttonPath->setGeometry(QRect(750, 180, 80, 30));
        buttonPath->setFont(font2);
        buttonPath->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius: 10px;\n"
"	color: rgb(52, 108, 212);\n"
"	background-color: rgb(175, 204, 255);\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"	color: rgb(40, 82, 160);\n"
"	background-color: rgb(155, 192, 255);\n"
"}"));
        stackedWidget->addWidget(addPage);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 960, 21));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lineEdit->setText(QString());
        labelEnter->setText(QCoreApplication::translate("MainWindow", "Enter:", nullptr));
        buttonSearch->setText(QCoreApplication::translate("MainWindow", "Search", nullptr));
        buttonCloseSearch->setText(QCoreApplication::translate("MainWindow", "Close", nullptr));
        labelName->setText(QCoreApplication::translate("MainWindow", "KIMIO", nullptr));
        buttonGoAdd->setText(QCoreApplication::translate("MainWindow", "Add Document", nullptr));
        buttonGoBack->setText(QCoreApplication::translate("MainWindow", "Back", nullptr));
        buttonCloseResult->setText(QCoreApplication::translate("MainWindow", "Close", nullptr));
        buttonCloseAdd->setText(QCoreApplication::translate("MainWindow", "Close", nullptr));
        buttonGoSearch->setText(QCoreApplication::translate("MainWindow", "Back", nullptr));
        editPath->setText(QString());
        labelPath->setText(QCoreApplication::translate("MainWindow", "Enter the path of the document here:", nullptr));
        labelSuccess->setText(QString());
        buttonPath->setText(QCoreApplication::translate("MainWindow", "Enter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
