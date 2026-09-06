#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStorageInfo>
#include <QComboBox>
#include <QProcess>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Configuração inicial da barra de progresso (começa zerada)
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::atualizarListaDrives() {
    // Limpa a lista atual do combobox para não duplicar itens
    ui->comboDrives->clear();
    ui->comboDrives->addItem("");
    ui->progressBar->setValue(0); // Zera a barra ao atualizar

    // Obtém a lista de todos os discos e partições presentes no sistema
    QList<QStorageInfo> drives = QStorageInfo::mountedVolumes();

    for (const QStorageInfo &drive : drives) {
        // Verifica se o drive está ativo, pronto para leitura e se é um dispositivo removível
        if (drive.isValid() && drive.isReady()) {

#ifdef Q_OS_WIN
            // No Windows, filtramos por drives USB/Removíveis
            // drives de CD/DVD ou partições fixas (C:) serão ignorados
            if (drive.fileSystemType() == "FAT32" || drive.fileSystemType() == "NTFS" || drive.fileSystemType() == "exFAT") {
                // Verificação adicional: se não for o drive C: (sistema) e for removível
                if (drive.rootPath() != "C:/" && !drive.isRoot()) {

                    // Cria um texto amigável, ex: "E: (MeuPendrive) [FAT32]"
                    QString nomeDrive = drive.rootPath().left(2); // Pega apenas o "E:"
                    QString label = drive.name().isEmpty() ? "Dispositivo Removível" : drive.name();
                    QString textoExibicao = QString("%1 (%2)").arg(nomeDrive, label);

                    // Adiciona ao ComboBox, guardando a letra real (ex: "E:") como dado oculto (UserData)
                    ui->comboDrives->addItem(textoExibicao, nomeDrive);
                }
            }
#endif
        }
    }

    // Se nenhum pendrive for encontrado, avisa o usuário
    if (ui->comboDrives->count() <2) {
        //ui->comboDrives->addItem("Nenhum pendrive detectado", "");
        ui->lbStatus->setText("Nenhum pendrive detectado");
    }else{
        ui->lbStatus->setText("Dispositivo encontrado. Pronto para limpar!");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    // Pega a letra do drive armazenada no item selecionado do ComboBox
    QString letraDrive = ui->comboDrives->currentData().toString();

    // Se o combobox estiver vazio ou com a mensagem de "Nenhum pendrive", interrompe
    if (letraDrive.isEmpty()) {
        //qDebug() << "Por favor, selecione um pendrive válido.";
        ui->lbStatus->setText("Por favor, selecione um pendrive válido.");
        return;
    }

    // --- NOVA VALIDAÇÃO DE SEGURANÇA ---
    // Cria um objeto de verificação apontando para a raiz do drive selecionado (ex: "E:/")
    QStorageInfo verificarDrive(letraDrive + "/");

    // Verifica se o drive ainda é válido, está pronto e ainda está montado no sistema
    if (!verificarDrive.isValid() || !verificarDrive.isReady()) {
        ui->lbStatus->setText("Erro: O pendrive foi removido ou não está acessível!");
        ui->progressBar->setValue(0);

        // Proativo: Já atualiza a lista do ComboBox para sumir com o drive fantasma
        MainWindow::atualizarListaDrives();
        return; // Interrompe a função aqui e não executa a limpeza falsa!
    }

    // --- Passo 1: Iniciando o processo ---
    ui->lbStatus->setText("Encerrando processos suspeitos...");
    ui->progressBar->setValue(15);
    QCoreApplication::processEvents(); // Força o Qt a atualizar o texto e a barra na tela

    // Mata processos (com a correção para não exibir erros chatos no console)
    QProcess::execute("cmd.exe", QStringList() << "/c" << "taskkill /f /im wscript.exe >nul 2>&1");
    QProcess::execute("cmd.exe", QStringList() << "/c" << "taskkill /f /im cmd.exe >nul 2>&1");

    // --- Passo 2: Deletando os arquivos ocultos .exe ---
    ui->lbStatus->setText("Removendo arquivos .exe falsificados da raiz...");
    ui->progressBar->setValue(45);
    QCoreApplication::processEvents();

    QProcess deletarExe;
    deletarExe.start("cmd.exe", QStringList() << "/c" << "del" << (letraDrive + "\\*.exe") << "/q" << "/f");
    deletarExe.waitForFinished(-1); // O -1 garante que ele espere terminar sem dar crash

    // --- Passo 3: Restaurando os arquivos ocultos (Attrib) ---
    ui->lbStatus->setText("Restaurando suas pastas ocultas (Isso pode demorar)...");
    ui->progressBar->setValue(75);
    QCoreApplication::processEvents();

    QProcess restaurarArquivos;
    restaurarArquivos.start("cmd.exe", QStringList() << "/c" << "attrib" << "-h" << "-r" << "-s" << "/s" << "/d" << (letraDrive + "\\*.*"));

    // --- Passo 4: Finalização ---
    if (restaurarArquivos.waitForFinished(-1)) {
        ui->progressBar->setValue(100);
        ui->lbStatus->setText("Sucesso! Pendrive limpo e pastas restauradas.");
        qDebug() << "Pendrive " << letraDrive << " limpo com sucesso!";
    } else {
        ui->lbStatus->setText("Ocorreu um problema ao finalizar a restauração.");
    }
}


void MainWindow::on_pushButton_clicked()
{
    MainWindow::atualizarListaDrives();
}

