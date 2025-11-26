#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>

/* ======================= Config DLL ======================= */

static HMODULE g_hDll = NULL;

#ifndef CALLCONV
#define CALLCONV WINAPI
#endif

/* ======================= Assinaturas da DLL ======================= */
typedef int (CALLCONV *AbreConexaoImpressora_t)(int, const char *, const char *, int);
typedef int (CALLCONV *FechaConexaoImpressora_t)(void);
typedef int (CALLCONV *ImpressaoTexto_t)(const char *, int, int, int);
typedef int (CALLCONV *Corte_t)(int);
typedef int (CALLCONV *ImpressaoQRCode_t)(const char *, int, int);
typedef int (CALLCONV *ImpressaoCodigoBarras_t)(int, const char *, int, int, int);
typedef int (CALLCONV *AvancaPapel_t)(int);
typedef int (CALLCONV *AbreGavetaElgin_t)(int, int, int);
typedef int (CALLCONV *AbreGaveta_t)(int, int, int);
typedef int (CALLCONV *SinalSonoro_t)(int, int, int);
typedef int (CALLCONV *ImprimeXMLSAT_t)(const char *, int);
typedef int (CALLCONV *ImprimeXMLCancelamentoSAT_t)(const char *, const char *, int);
typedef int (CALLCONV *InicializaImpressora_t)(void);

/* ======================= Ponteiros ======================= */
static AbreConexaoImpressora_t        AbreConexaoImpressora;
static FechaConexaoImpressora_t       FechaConexaoImpressora;
static ImpressaoTexto_t               ImpressaoTexto;
static Corte_t                        Corte;
static ImpressaoQRCode_t              ImpressaoQRCode;
static ImpressaoCodigoBarras_t        ImpressaoCodigoBarras;
static AvancaPapel_t                  AvancaPapel;
static AbreGavetaElgin_t              AbreGavetaElgin;
static AbreGaveta_t                   AbreGaveta;
static SinalSonoro_t                  SinalSonoro;
static ImprimeXMLSAT_t                ImprimeXMLSAT;
static ImprimeXMLCancelamentoSAT_t    ImprimeXMLCancelamentoSAT;
static InicializaImpressora_t         InicializaImpressora;

/* ======================= Configuração ======================= */
static int   g_tipo      = 1;
static char  g_modelo[64] = "i9";
static char  g_conexao[128] = "USB";
static int   g_parametro = 0;
static int   g_conectada = 0;

/* ======================= Utilidades ======================= */

#define LOAD_FN(h, name) do { \
    name = (name##_t)GetProcAddress((HMODULE)(h), #name); \
    if (!(name)) { fprintf(stderr, "Falha ao resolver símbolo %s (erro=%lu)\n", #name, GetLastError()); return 0; } \
} while (0)

static void flush_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* ======================= Funções DLL ======================= */

static int carregarFuncoes(void) {

    g_hDll = LoadLibraryA("E1_Impressora01.dll");
    if (!g_hDll) {
        fprintf(stderr, "Erro ao carregar E1_Impressora01.dll (erro=%lu)\n", GetLastError());
        return 0;
    }

    LOAD_FN(g_hDll, AbreConexaoImpressora);
    LOAD_FN(g_hDll, FechaConexaoImpressora);
    LOAD_FN(g_hDll, ImpressaoTexto);
    LOAD_FN(g_hDll, Corte);
    LOAD_FN(g_hDll, ImpressaoQRCode);
    LOAD_FN(g_hDll, ImpressaoCodigoBarras);
    LOAD_FN(g_hDll, AvancaPapel);
    LOAD_FN(g_hDll, AbreGavetaElgin);
    LOAD_FN(g_hDll, AbreGaveta);
    LOAD_FN(g_hDll, SinalSonoro);
    LOAD_FN(g_hDll, ImprimeXMLSAT);
    LOAD_FN(g_hDll, ImprimeXMLCancelamentoSAT);
    LOAD_FN(g_hDll, InicializaImpressora);

    return 1;
}

static void liberarBiblioteca(void) {
    if (g_hDll) {
        FreeLibrary(g_hDll);
        g_hDll = NULL;
    }
}

/* ======================= Menu ======================= */

static void exibirMenu(void) {
    printf("\n======================\n");
    printf("=========MENU=========\n");
    printf("======================\n");
    printf("1 - Configurar Conexao\n");
    printf("2 - Abrir Conexao\n");
    printf("3 - Impressao Texto\n");
    printf("4 - Impressao QRCode\n");
    printf("5 - Impressao Cod Barras\n");
    printf("6 - Impressao XML SAT\n");
    printf("7 - Impressao XML Canc SAT\n");
    printf("8 - Abrir Gaveta Elgin\n");
    printf("9 - Abrir Gaveta\n");
    printf("10 - Sinal Sonoro\n");
    printf("0 - Fechar Conexao e Sair\n");
    printf("======================\n");
    printf("Escolha uma opcao: ");
}

/* ======================= Configuração ======================= */

static int configurarConexao(int *tipo, char *modelo, char *conexao, int *parametro)
{
    printf("Tipo de entrada\n");
    printf("1 - USB\n");
    printf("2 - RS232\n");
    printf("3 - TCP/IP\n");
    printf("4 - Bluetooth\n");
    printf("5 - Impressoras acopladas\n");
    scanf("%d", tipo);

    printf("============================\n");
    printf("Modelo:\n");
    printf("i7 / i7Plus / i8 / i9 / ix\n");
    scanf("%s", modelo);

    printf("============================\n");
    printf("Tipo de Conexao:\n");
    printf("USB / RS232 / TCP/IP / Bluetooth\n");
    scanf("%s", conexao);

    *parametro = 0;

    if (*tipo == 1 && strcmp(modelo, "i9") == 0 && strcmp(conexao, "USB") == 0) {
        printf("Configuração correta!\n");
        return 0;
    }

    printf("Configuração diferente da impressora.\n");
    return 1;
}

/* ======================= Ações ======================= */

static void abrirConexao(void) {
    int ret = AbreConexaoImpressora(g_tipo, g_modelo, g_conexao, g_parametro);
    if (ret == 0) {
        printf("Conexão aberta com sucesso!\n");
        g_conectada = 1;
        InicializaImpressora();
    } else {
        printf("Falha ao abrir conexão (%d)\n", ret);
    }
}

static void fecharConexao(void) {
    if (g_conectada) {
        FechaConexaoImpressora();
        g_conectada = 0;
        printf("Conexão encerrada.\n");
    } else {
        printf("Nenhuma conexão ativa.\n");
    }
}

static void imprimirTexto(void) {
    char texto[256];
    flush_entrada();

    printf("Digite o texto a ser impresso: ");
    fgets(texto, sizeof(texto), stdin);
    texto[strcspn(texto, "\n")] = 0;

    ImpressaoTexto(texto, 0, 0, 0);
    AvancaPapel(5);
    Corte(2);

    printf("Texto impresso!\n");
}

static void imprimirQRCode(void) {
    char conteudo[256];
    flush_entrada();

    printf("Conteúdo do QRCode: ");
    fgets(conteudo, sizeof(conteudo), stdin);
    conteudo[strcspn(conteudo, "\n")] = 0;

    ImpressaoQRCode(conteudo, 6, 4);
    AvancaPapel(5);
    Corte(2);

    printf("QRCode impresso!\n");
}

static void imprimirCodigoBarras(void) {
    ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3);
    AvancaPapel(5);
    Corte(2);
    printf("Código de barras impresso!\n");
}

static void imprimirXMLSAT(void)
{
    FILE *file = fopen("./XMLSAT.xml", "r");
    if (!file) {
        perror("Erro ao abrir XMLSAT.xml");
        return;
    }

    fseek(file, 0, SEEK_END);
    long tamanho = ftell(file);
    rewind(file);

    char *conteudo = malloc(tamanho + 1);
    fread(conteudo, 1, tamanho, file);
    conteudo[tamanho] = 0;
    fclose(file);

    ImprimeXMLSAT(conteudo, 0);
    free(conteudo);
    AvancaPapel(5);
    Corte(2);
    printf("XML SAT impresso!\n");
}

static void imprimirXMLCancelamentoSAT(void)
{
    char assinatura[] =
        "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
        "jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
        "SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
        "Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
        "p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
        "YVFCDtYR9Hi5qgdk31v23w==";

    FILE *file = fopen("./CANC_SAT.xml", "r");
    if (!file) {
        perror("Erro ao abrir CANC_SAT.xml");
        return;
    }

    fseek(file, 0, SEEK_END);
    long tamanho = ftell(file);
    rewind(file);

    char *conteudo = malloc(tamanho + 1);
    fread(conteudo, 1, tamanho, file);
    conteudo[tamanho] = 0;
    fclose(file);

    ImprimeXMLCancelamentoSAT(conteudo, assinatura, 0);
    free(conteudo);
    
    AvancaPapel(5);
    Corte(2);

    printf("XML Cancelamento impresso!\n");
}

static void abrirGavetaElginOpc(void) {
    AbreGavetaElgin(1, 50, 50);
    printf("Gaveta Elgin aberta!\n");
}

static void abrirGavetaOpc(void) {
    AbreGaveta(1, 5, 10);
    printf("Gaveta aberta!\n");
}

static void emitirSinalSonoro(void) {
    SinalSonoro(4, 50, 5);
    printf("Sinal sonoro emitido!\n");
}

/* ======================= MAIN ======================= */

int main(void) {

    if (!carregarFuncoes()) return 1;

    int opcao = -1;
    int configFeita = 0;
    int conexaoAberta = 0;

    while (1) {
        exibirMenu();
        scanf("%d", &opcao);

        if (!configFeita && opcao > 1 && opcao != 0) {
            printf("Você precisa configurar a conexão primeiro!\n");
            system("pause");
            system("cls");
            continue;
        }

        if (configFeita && !conexaoAberta && opcao > 2 && opcao != 0) {
            printf("Você precisa abrir a conexão antes!\n");
            system("pause");
            system("cls");
            continue;
        }
            switch (opcao) {

                case 1:
                    if (configurarConexao(&g_tipo, g_modelo, g_conexao, &g_parametro) == 0)
                        configFeita = 1;
                    break;

                case 2:
                    abrirConexao();
                    conexaoAberta = 1;
                    break;

                case 3:
                    imprimirTexto();
                    break;

                case 4:
                    imprimirQRCode();
                    break;

                case 5:
                    imprimirCodigoBarras();
                    break;

                case 6:
                    imprimirXMLSAT();
                    break;

                case 7:
                    imprimirXMLCancelamentoSAT();
                    break;

                case 8:
                    abrirGavetaElginOpc();
                    break;

                case 9:
                    abrirGavetaOpc();
                    break;

                case 10:
                    emitirSinalSonoro();
                    break;

                case 0:
                    fecharConexao();
                    liberarBiblioteca();
                    printf("Encerrando o sistema...\n");
                    return 0;

                default:
                    printf("Opção inválida!\n");
            }
        }
        system("cls");
}