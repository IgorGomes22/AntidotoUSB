# 🛡️ AntidotoUSB v1.0.0
![image](https://github.com/IgorGomes22/AntidotoUSB/blob/main/interface.png)

O **AntidotoUSB** é um utilitário leve e eficaz desenvolvido em **C++** com o framework **Qt 6**. Ele foi criado para combater o famoso "vírus de atalho" (falsos executáveis `.exe`) que infecta dispositivos de armazenamento removíveis, ocultando os arquivos legítimos do usuário.

---

## 🚀 Como Usar

1. Baixe o executável único na aba [Releases]([https://github.com](https://github.com/IgorGomes22/AntidotoUSB/releases/tag/v1.0.0)).
2. Conecte o seu pendrive infectado no computador.
3. Abra o **AntidotoUSB** e selecione a letra correspondente à repartição do seu pendrive.
4. Clique em **Limpar** para iniciar o processo de varredura e recuperação.

> ⚠️ **ATENÇÃO:** Todos os arquivos com a extensão `.exe` que estiverem soltos na raiz do pendrive serão deletados permanentemente, pois este é o formato padrão utilizado pelo vírus para induzir o clique.

---

## 🛠️ O que ele faz nos bastidores?

* **Encerra ameaças na memória:** Tenta paralisar scripts maliciosos ativos (`wscript.exe`, `cmd.exe`) usando chamadas de sistema.
* **Elimina arquivos maliciosos:** Executa a deleção em lote dos falsos executáveis `.exe` criados pelo vírus no drive USB.
* **Restaura os arquivos ocultos:** Utiliza comandos de sistema (`attrib -h -r -s`) para remover as propriedades de ocultação severa aplicadas às suas pastas legítimas.

---

## 💻 Tecnologias Utilizadas

* **C++ moderno** (Lógica de controle e segurança de processos)
* **Qt 6 Framework** (Interface gráfica integrada e classes de sistema como `QStorageInfo` e `QProcess`)
* **Enigma Virtual Box** (Para o encapsulamento virtual de todas as dependências em um único arquivo `.exe` portátil)

---

## 🔒 Nota de Segurança / Antivírus
Por se tratar de uma ferramenta que executa comandos administrativos em lote (manipulação de atributos de arquivos e finalização de processos), alguns antivírus heurísticos podem exibir alertas ou travar a execução. O código-fonte está totalmente aberto acima para auditoria e o aplicativo é 100% seguro.
