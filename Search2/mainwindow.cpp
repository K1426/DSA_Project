#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "algo.h"
#include "QFileInfo"
#include "QDesktopServices"
#include "QProcess"

std::string input_dir = "D:\\DSA_Project\\DSA_Project\\sample\\", forward = "forward_index.txt", inverted = "inverted_index.txt";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // Ensure directory exists
    if (!fsys::exists(input_dir) || !fsys::is_directory(input_dir))
    {
        std::cerr << "Error: Input directory does not exist: " << input_dir << "\n";
    }

    int processed = make_things(input_dir);

    if (processed > 0)
    {
        std::cout << "\nCompleted Successfully!\n";
        std::cout << "Files processed: " << processed << "\n";
        std::cout << "Lexicon size: " << lexicon.getSize() << "\n";
        std::cout << "Next available ID: " << current_wordID + 1 << "\n";
        std::cout << "Output saved in:\n";
        std::cout << "  - " << lexicon_file << "\n";
        std::cout << "  - " << forward_index_file << "\n";
        build_inverted_index(forward, inverted);
    }

    load_barrels(inverted);
    std::cout << "Loaded barrels\n";
    ui->setupUi(this);
    ui->resultsBrowser->setReadOnly(true);
    ui->resultsBrowser->setOpenLinks(false);
    ui->resultsBrowser->setOpenExternalLinks(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    //autocompletion
    ui->resultsBrowser->setText("");
    if (arg1 != "")
    {
        std::vector<std::string> res = lexicon.printAutoSuggestions(arg1.toStdString());
        for (std::string& suggestion : res) ui->resultsBrowser->insertPlainText(QString::fromStdString(suggestion + "\n"));
        res.clear();
    }
}


void MainWindow::on_buttonSearch_clicked()
{
    //search
    ui->resultsBrowser->setText("");
    std::string query = ui->lineEdit->text().toStdString();
    std::vector<std::pair<int, float>> results = multi_search(query);
    for (auto& [docID, rank] : results)
    {
        ui->resultsBrowser->append(QString::fromStdString("<a href=\"file:" + paths[docID - 1]+ "\">" + titles[docID - 1] + "</a><br>"));
    }
}


void MainWindow::on_buttonPath_clicked()
{
    std::string path = ui->editPath->text().toStdString();
    ui->editPath->setText("");
    std::ifstream src(path, std::ios::binary);
    if (!fsys::exists(path))
    {
        ui->labelSuccess->setText("This path does not exist.");
    }
    else if (!src.is_open())
    {
        ui->labelSuccess->setText("Could not open file.");
    }
    else
    {
        std::string copyname = path.substr(path.rfind('\\'));
        if (fsys::exists(input_dir + copyname)) copyname = "copy_" + copyname;
        std::ofstream dest(input_dir + copyname, std::ios::binary);
        dest << src.rdbuf();
        src.close();
        dest.close();
        ui->labelSuccess->setText("File successfully uploaded.");
    }
}


void MainWindow::on_resultsBrowser_anchorClicked(const QUrl &arg1)
{
    ui->stackedWidget->setCurrentIndex(1);
    std::string path = arg1.toLocalFile().toStdString(), content = "";
    if (path.substr(path.rfind('.')) == ".json") content = parse_json(path.c_str());
    else content = parse_txt(path);
    ui->resultsEdit->setPlainText(QString::fromStdString(content));
}


void MainWindow::on_buttonGoAdd_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::on_buttonGoSearch_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_buttonGoBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_buttonCloseAdd_clicked()
{
    QApplication::quit();
}


void MainWindow::on_buttonCloseSearch_clicked()
{
    QApplication::quit();
}


void MainWindow::on_buttonCloseResult_clicked()
{
    QApplication::quit();
}

