# Projeto Elgin

<img src="./elgin-logo-0-1.png" alt="Logo da Elgin" width="300">
<p style="font-size: 15">Um clone do sistema de comunicação com impressoras de cupom fiscal da Elgin</p>
<!--![Elgin](./img/elgin-logo-0-1.png) Um clone do sistema de comunicação com impressoras de cupom fiscal da Elgin-->

## 📌 Índice

-   [Funcionalidades do Projeto](#funcionalidades-do-projeto)
-   [Função geral](#Função-geral)
-   [Como Rodar Este Projeto](#como-rodar-este-projeto)
-   [Tecnologias Utilizadas](#tecnologias-utilizadas)
-   [Pessoas Autoras](#pessoas-autoras)

------------------------------------------------------------------------

## 📱 Funcionalidades do Projeto

-   [x] AbreConexaoImpressora()
-   [x] FechaConexaoImpressora()
-   [x] ImpressaoTexto()
-   [x] Corte()
-   [x] ImpressaoQRCode()
-   [x] ImpressaoCodigoBarras()
-   [x] AvancaPapel()
-   [x] AbreGavetaElgin()
-   [x] AbreGaveta()
-   [x] SinalSonoro()
-   [x] ImprimeXMLSAT()
-   [x] ImprimeXMLCancelamentoSAT()

------------------------------------------------------------------------

## 💻Como rodar o projeto?
```bash
# Clona este repositório
# Abrir o projeto no Visual Studio
```
------------------------------------------------------------------------
## 🔍Função geral do projeto

1. Estabelecer comunicação com a impressora
   - Abrir conexão via USB, Serial ou IP
   - Fechar a conexão quando terminar

2. Enviar comandos de impressão
   - Imprimir texto simples
   - Imprimir QR Code
   - Imprimir Código de Barras
   - Imprimir XML SAT e XML de cancelamento

3. Controlar funções físicas da impressora
   - Cortar o papel
   - Avançar o papel
   - Emitir sinal sonoro
   - Abrir gaveta de dinheiro (Elgin ou externa)


------------------------------------------------------------------------

## 🔨 Tecnologias Utilizadas

-   Linguagem C
-   Visual Studio 2022
-   WinAPI
-   DLL de comunicação oficial da Elgin
-   Ambiente Windows 64 bits

------------------------------------------------------------------------

## 👤 Pessoas Autoras

-   João Pedro -- Desenvolvimento
-   Kaun -- Desenvolvimento
-   Isaque
-   Sandrack
-   ~sıcʞ 愛

