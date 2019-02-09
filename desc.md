
# Magos
Esse projeto é um programa para geração e resolução de labirintos. O 
programa produz imagens no formato .png mostrando todo o processo de 
geração do labirinto e da IA resolvendo-o.  

A implementação foi feita em c++ com ajuda das bibliotecas stb_image e boost_program_options.

# Desafios 
O primeiro desafio implementar as classes responsáveis para fazer o 
projeto, a primeira classes foi o canvas, que é resposável por fazer os 
desenhos e armazenar em um vetor de pixel, depois foram as classes do projeto em si, como o builder e o render a classe MagosGame. Cada uma dessas classes tem um desafio principal.  
Nas classes dos processos de geração e resolução eu escolhi fazer um 
método inicializador para o algoritmo, um método para gerar a próxima 
etapa, um para retornar o número da atual, e um outro para verificar se 
o processo acabou. Eu poderia fazer de outra forma,uma forma melhor não 
precisaria do inicializador e eu poderia trabalha retornado um objeto do 
tipo Resultado no next_step, no entanto eu decidi fazer da forma mais 
rápida, pois, como eu não tinha compromisso com o projeto (fiz só por 
diversão), eu queria focar em outros "parâmetros". 
Na classe Maze, eu trabalhei com as celúlas armazenando um octeto 
(um byte), assim eu poderia armazenado o estado das células nos algoritmos 
e o estado das paredes - apenas um bit ficou sem uso. Para fazer as 
operações com bit eu criei os métodos set, unset e get.  
O algoritmos que eu usei foram os mais simples, os dois usam busca em 
profundidade. Em um momento posterior eu vou me dedicar a aprender como 
funcionam os outros algoritmos e quando usa-los.  
De maneira geral, o projeto 'puro' se tornou bastante simples, pois foi 
dada boa parte da estrutura.

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
