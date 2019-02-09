# Magos
Magos é um projeto dividido em duas partes:  
- Gerar um labirinto.
- Resolver o labirinto.  
A implementação foi feita em c++ com ajuda das bibliotecas stb_image e boost_program_options.

# Como usar
O programa possui valores padrão para todos os argumentos e o arquivo de 
configuração está incluso, porém é necessário que você crie as pastas onde 
ficará a saída antes de continuar. Se você não alterou o caminho de saída 
padrão, crie as pastas `builder` e `solver` dentro do diretório raíz do 
projeto, feito isso você pode executar com `./magos`.  
Você tem disponível várias opções (alterar tamanho da margem, cores, 
resolução da imagens, tamanho do labirinto, etc), para ver as opções de 
linha de comando use `./magos --help`, para ver as outras opções abra o 
arquivo de configuração `autoconfig.cfg` e vá alterando ao seu gosto.  
No final da execução o programa irá gerar as imagens nos diretórios 
correspondentes.  
Para gerar um video com as imagens obtidas no processo, você pode usar 
algo semelhante a isso:
- Gerando as imagens do builder: 
´ffmpeg -framerate 5 -i builder/building_%05d.png -r 30 -pix_fmt yuv420p -filter_complex "[0]trim=0:5[hold];[0][hold]concat[extended];[extended][0]overlay" building.mp4´ 
- Gerando as imagens do solver: 
´ffmpeg -framerate 5 -i solver/solving_%05d.png -r 30 -pix_fmt yuv420p -filter_complex "[0]trim=0:5[hold];[0][hold]concat[extended];[extended][0]overlay" solving.mp4´ 
- Juntado os dois vídeos:  
`ffmpeg -f concat -i concat.txt -c copy magos.mp4`  

Obs.: a resolução do video será a resolução das imagens geradas.
