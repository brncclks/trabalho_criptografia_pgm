#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 8

// Função auxiliar: Copia o cabeçalho de texto do PGM P2.
int copy_pgm_header(FILE *in, FILE *out) {
    char buffer[256];
    int line_count = 0;
 
    // O cabeçalho PGM "P2" possui 3 componentes numéricos (Largura, Altura, MaxVal).
    while (fgets(buffer, sizeof(buffer), in)) {
        fputs(buffer, out);
        // Ignora linhas de comentários que começam com '#'
         if (buffer[0] == '#' || buffer[0] == '\n' ||
            buffer[0] == '\r') {
            continue;
        }

        line_count++;

        if (line_count == 3) {
            return 1;
        }
    }

    return 0;
}

// Tarefa 1: Implementar o modo ECB (Blocos de 8 pixels)
void process_ecb(const char *input_filename, const char *output_filename, unsigned char *key) {
    FILE *in = fopen(input_filename, "r"); // Modo leitura de texto
    FILE *out = fopen(output_filename, "w"); // Modo escrita de texto
 
    if (!in || !out) {
        printf("Erro ao abrir arquivos para ECB.\n");
        return;
    }

    copy_pgm_header(in, out);

    int pixel_block[BLOCK_SIZE];
    int count = 0;
    int pixel_val;

    printf("Iniciando encriptacao ECB...\n");
    // Lê o arquivo formatado, inteiro por inteiro
    while (fscanf(in, "%d", &pixel_val) == 1) {
        pixel_block[count] = pixel_val;
        count++;

        // Quando o bloco enche, processamos e escrevemos
        if (count == BLOCK_SIZE) { 
            for (int i = 0; i < BLOCK_SIZE; i++) {
            pixel_block[i] = pixel_block[i] ^ key[i];
        }

            for (int i = 0; i < BLOCK_SIZE; i++) {
                fprintf(out, "%d\n", pixel_block[i]);
            }
            count = 0; // Reseta o contador para o próximo bloco
        }
    }

    // TODO (Aluno): Tratar o bloco final incompleto (Padding / Resto)
    // Se o arquivo terminar e 'count' for maior que 0, significa que 
    // sobraram alguns pixels que não formaram um bloco inteiro.
    if (count > 0) {
        for (int i = 0; i < count; i++) {
            pixel_block[i] = pixel_block[i] ^ key[i];
            fprintf(out, "%d\n", pixel_block[i]);
        }
    }

    fclose(in);
    fclose(out);
}

// Tarefa 2: Implementar a Cifra de Fluxo (Pixel a Pixel)
void process_stream(const char *input_filename, const char *output_filename, unsigned int seed_key) {
    FILE *in = fopen(input_filename, "r");
    FILE *out = fopen(output_filename, "w");
 
    if (!in || !out) {
        printf("Erro ao abrir arquivos para Stream.\n");
        return;
    }

    copy_pgm_header(in, out);

    srand(seed_key); 
    int pixel_val;
 
    printf("Iniciando encriptacao de Fluxo...\n");

    // Lê continuamente, número a número
    while (fscanf(in, "%d", &pixel_val) == 1) {
 
        // TODO (Aluno): Gerar um keystream e aplicar XOR
        int keystream = rand() % 256;
        int encrypted_val = pixel_val ^ keystream;
        fprintf(out, "%d\n", encrypted_val);
        //
    }

    fclose(in);
    fclose(out);
}

int main() { 
    const char *input_img = "imagem_p2.pgm"; 
    const char *ecb_out = "imagem_ecb_p2.pgm";
    const char *stream_out = "imagem_stream_p2.pgm";

    // Chave estática de 8 bytes para o ECB
    unsigned char key[BLOCK_SIZE] = {0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE, 0xBA, 0xBE};
 
    // Chave (semente) para a cifra de fluxo
    unsigned int stream_key = 12345;

    process_ecb(input_img, ecb_out, key);
    process_stream(input_img, stream_out, stream_key);

    printf("Processamento concluido. Verifique os arquivos gerados!\n");
 
    return 0;
}