#include <stdio.h>
#include <string.h>
#include <float.h>

/*  Função que retorna o lucro máximo obtido através de no máximo
    k transações compra-venda de ações em um período de n instantes 
    de tempo */

float profit(float price[], int n, int k) {
    /*  i, j: Variáveis de loop
        profit: Matriz em que profit[i][j] representa o máximo lucro
            obtido usando i transações até o dia j
        keep: Matriz que armazena os instantes de compra e venda,
            se houverem sido lucrativos, usando i transações e considerando
            j instantes de tempo.
    */
    int i, j;
    float profit[k+1][n+1];
    int keep[k+1][n+1][2];

    memset(keep, 0, sizeof(keep[0][0]) * (k+1) * (n+1));

    /*  Usando-se zero instantes de tempo, não pode haver transação, 
        portanto o lucro é zero */
    for (i = 0; i <= k; i++)
        profit[i][0] = 0;

    /*  Se não há transações realizadas, o lucro é zero */
    for (j= 0; j <= n; j++)
        profit[0][j] = 0;

    /*  Preenche a matriz profit de maneira bottom-up */
    for (i = 1; i <= k; i++) {
        for (j = 1; j < n; j++) {
            /*  m: variável de loop interno de comparação 
                m_max: instante de tempo de compra de ações mais lucrativo
                    usando i transações e j dias
                j_max: instante de tempo de venda de ações mais lucrativo
                    usando i transações e j dias
                max: máximo lucro obtido nessa iteração */
            int m;
            int m_max = -1;
            int j_max = -1;
            float max = FLT_MIN;
            /*  Compara os lucros obtidos com compra em m venda 
                em j para todo m < j. Atualiza os valores max, m_max, j_max */
            for (m = 0; m < j; m++) {
                if(price[j] - price[m] + profit[i-1][m] > max) {
                    max = price[j] - price[m] + profit[i-1][m];
                    m_max = m;
                    j_max = j;
                }
            }
            /*  Se o lucro máximo obtido na iteração do instante j for
                maior que o lucro obtido no instante j-1, atualizar o lucro,
                atualizar os momentos de compra e venda na matriz keep */
            if(max > profit[i][j-1]) {
                profit[i][j] = max;
                if(m_max != -1 && j_max != -1) {
                    keep[i][j][0] = m_max;
                    keep[i][j][1] = j_max;
                }
            /* Caso contrário, o lucro anterior é mantido */
            } else {
                profit[i][j] = profit[i][j-1];
            }
        }
    }

    /* DEBUG */
    // for(i=0; i < k+1; i++) {
    //     for(j=0; j < n; j++) {
    //         printf("%d-%d ", keep[i][j][0], keep[i][j][1]);
    //     }
    //     printf("\n");
    // }

    /*  Extração dos instantes de compra e venda a partir de keep.
        t: número de transações identificadas
        aux: auxiliar para identificar intervalos de transação disjuntos
        transactions: matriz que armazena os momento de transação */
    int t = 0;
    int aux = n-1;
    int transactions[k][2];

    /*  Extração de no máximo k transações */
    while(t < k && aux > -1) {
        /*  Busca a última transação mais lucrativa usando k-t transações */
        while(keep[k-t][aux][0] == keep[k-t][aux][1] && aux > -1)
            aux--;
        /*  Salva os instantes de compra e venda da transação */
        transactions[k-t-1][0] = keep[k-t][aux][0];
        transactions[k-t-1][1] = keep[k-t][aux][1];
        /*  Busca o índice da última transação mais lucrativa 
            usando k-t-1 transações */
        aux = keep[k-t][aux][0] - 1;
        t++;
        while(keep[k-t][aux][0] == keep[k-t][aux][1] && aux > -1)
            aux--;
    }

    /*  Imprime a sequência de transações a ser realizada*/
    for(i=k-t; i < k; i++) {
        printf("Buy at %.2f (time %d) and Sell for %.2f (time %d) ", price[transactions[i][0]],
                                                                transactions[i][0],
                                                                price[transactions[i][1]],
                                                                transactions[i][1]);
        printf("with Profit %.2f\n", price[transactions[i][1]]- price[transactions[i][0]]);
    }

    return profit[k][n-1];
}

void main() {
    /*  k: Valor máximo de transações permitidas */
    int k = 5;

    /* Vetores de preços de ações de tests */
    float price1[] = {10, 22, 5, 75, 65, 80};
    float price2[] = {12, 14, 17, 10, 14, 13, 12, 15};
    float price3[] = {100, 30, 15, 10, 8, 25, 80};
    float price4[] = {90, 80, 70, 60, 50};

    /*  bitcoin: Vetor com o preço diário de 1 bitcoin em dólar 
        no último ano (13/07/2016 a 12/07/2017) */
    float bitcoin[] = {2332.77, 2385.89, 2525.25, 2572.61, 2520.27, 2608.59, 2608.1, 2602.87, 2561.0, 2498.56, 2436.4, 2492.6, 2539.24, 2567.56, 2553.03, 2478.45, 2590.57, 2607.25, 2738.52, 2707.34, 2691.03, 2709.43, 2591.26, 2549.03, 2655.35, 2514.01, 2469.57, 2499.58, 2716.88, 2680.91, 2953.22, 2942.41, 2828.14, 2807.44, 2720.49, 2869.38, 2690.84, 2512.4, 2547.79, 2493.72, 2404.03, 2288.33, 2187.19, 2255.36, 2159.43, 2054.08, 2196.27, 2320.89, 2446.24, 2321.37, 2191.56, 2043.19, 2067.03, 1984.24, 1897.37, 1818.7, 1726.73, 1741.7, 1808.44, 1800.86, 1723.12, 1845.76, 1780.37, 1756.52, 1723.89, 1596.92, 1579.47, 1556.81, 1540.87, 1490.72, 1453.78, 1421.03, 1348.3, 1321.87, 1317.84, 1317.74, 1281.88, 1265.99, 1250.45, 1209.63, 1231.92, 1222.71, 1229.42, 1211.08, 1212.13, 1193.77, 1183.25, 1172.61, 1167.3, 1170.33, 1201.02, 1204.81, 1187.46, 1187.3, 1176.57, 1172.65, 1178.94, 1125.81, 1134.14, 1145.52, 1102.95, 1080.61, 1071.71, 1026.64, 1042.21, 1046.08, 1044.58, 972.05, 974.01, 936.54, 1038.45, 1050.05, 1120.65, 1055.36, 1037.24, 976.73, 1099.69, 1180.16, 1251.33, 1240.16, 1232.16, 1221.78, 1176.62, 1116.32, 1189.36, 1150.35, 1223.23, 1273.21, 1267.47, 1254.29, 1277.43, 1250.71, 1224.68, 1180.04, 1180.72, 1163.78, 1144.27, 1170.41, 1172.71, 1117.27, 1114.8, 1079.28, 1048.69, 1054.76, 1049.21, 1026.12, 1007.65, 1006.21, 991.74, 998.88, 1003.52, 988.9, 995.63, 1064.7, 1062.32, 1040.14, 1028.4, 1043.52, 1031.33, 1011.46, 990.0, 970.94, 920.96, 920.15, 922.07, 919.81, 918.36, 902.39, 891.92, 910.68, 925.5, 922.21, 895.55, 898.17, 888.34, 909.37, 830.95, 821.78, 818.14, 825.14, 803.74, 775.18, 908.12, 902.44, 913.24, 908.17, 903.49, 1014.24, 1156.73, 1044.4, 1021.6, 998.62, 963.66, 960.63, 972.53, 975.13, 934.83, 908.35, 896.9, 899.65, 922.18, 864.89, 834.18, 800.64, 792.25, 790.69, 791.01, 785.17, 778.96, 780.07, 780.0, 780.65, 770.04, 774.75, 773.02, 769.94, 768.08, 764.21, 758.72, 773.39, 771.64, 778.25, 757.54, 746.05, 736.28, 736.33, 732.48, 735.44, 741.51, 740.44, 744.62, 751.74, 739.64, 731.26, 751.88, 751.83, 740.71, 744.88, 711.17, 705.79, 702.0, 705.2, 716.75, 715.55, 722.84, 709.83, 703.09, 710.74, 703.81, 703.52, 689.12, 742.35, 730.07, 701.34, 702.64, 714.12, 690.29, 688.0, 678.21, 657.68, 654.0, 657.16, 657.62, 633.14, 630.83, 630.66, 638.13, 639.41, 641.82, 639.08, 640.31, 637.01, 636.03, 640.87, 619.24, 616.82, 619.17, 617.34, 612.61, 612.47, 610.22, 612.05, 610.97, 613.95, 609.93, 605.72, 605.02, 606.24, 608.02, 600.81, 602.75, 602.96, 596.2, 597.28, 603.59, 609.25, 609.87, 606.28, 607.22, 607.25, 610.59, 608.84, 608.02, 607.0, 623.42, 622.93, 626.35, 614.63, 610.57, 606.51, 608.99, 598.59, 575.55, 572.41, 575.55, 577.59, 574.11, 574.07, 569.83, 579.45, 577.75, 580.18, 583.41, 586.77, 581.31, 581.94, 576.08, 574.34, 573.71, 577.76, 567.24, 570.49, 585.59, 587.36, 588.8, 592.12, 587.65, 591.04, 592.74, 587.77, 575.03, 578.28, 566.33, 548.66, 606.4, 624.6, 655.1, 657.01, 655.11, 654.49, 651.63, 654.23, 661.26, 655.41, 650.73, 664.92, 665.23, 672.81, 672.74, 679.81, 661.99, 663.78, 659.17, 652.92, 664.8};

    /*  Impressão dos índices dos dias em que as transações
        devem ser realizadas e do lucro obtido ao final */
    int  n = sizeof(bitcoin)/ sizeof(bitcoin[0]);
    printf("\nMaximum profit: %.2f\n", profit(bitcoin, n, k));
}
