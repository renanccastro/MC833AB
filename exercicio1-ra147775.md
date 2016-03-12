#Exercício 1: FERRAMENTAS
##ping
O comando ping informa se um host na internet está alcançável ou não.
Podemos utilizar os parâmetros da linha de comando para especificar o seu comportamento. O parâmetro -c, define quantos pacotes serão enviados(e recebidos) pela ferramenta até o fim do programa.

Foi executado ping nos seguintes hosts:

* [Universidade de Cambridge](www.cam.ac.uk):

	~~~bash
	➜  Downloads  ping -c 10 www.cam.ac.uk
	PING www.cam.ac.uk (131.111.150.25): 56 data bytes
	64 bytes from 131.111.150.25: icmp_seq=0 ttl=45 time=236.255 ms
	64 bytes from 131.111.150.25: icmp_seq=1 ttl=46 time=318.006 ms
	64 bytes from 131.111.150.25: icmp_seq=2 ttl=46 time=338.871 ms
	64 bytes from 131.111.150.25: icmp_seq=3 ttl=46 time=257.205 ms
	64 bytes from 131.111.150.25: icmp_seq=4 ttl=46 time=342.319 ms
	64 bytes from 131.111.150.25: icmp_seq=5 ttl=46 time=227.454 ms
	64 bytes from 131.111.150.25: icmp_seq=6 ttl=46 time=346.624 ms
	64 bytes from 131.111.150.25: icmp_seq=7 ttl=46 time=243.028 ms
	64 bytes from 131.111.150.25: icmp_seq=8 ttl=46 time=258.779 ms
	64 bytes from 131.111.150.25: icmp_seq=9 ttl=46 time=279.523 ms
	
	--- www.cam.ac.uk ping statistics ---
	10 packets transmitted, 10 packets received, 0.0% packet loss
	round-trip min/avg/max/stddev = 227.454/284.806/346.624/44.736 ms
	~~~

	* Tempos de ida e volta mínimo: 227.454 ms
	* Médio: 284.806 ms
	* Máximo: 346.624 ms
	
* [Universidade de Campinas](www.unicamp.br):

	~~~bash
	➜  Downloads  ping -c 10 www.unicamp.br
	PING cerejeira.unicamp.br (143.106.10.174): 56 data bytes
	64 bytes from 143.106.10.174: icmp_seq=0 ttl=56 time=11.882 ms
	64 bytes from 143.106.10.174: icmp_seq=1 ttl=56 time=50.932 ms
	64 bytes from 143.106.10.174: icmp_seq=2 ttl=56 time=13.968 ms
	64 bytes from 143.106.10.174: icmp_seq=3 ttl=56 time=11.002 ms
	64 bytes from 143.106.10.174: icmp_seq=4 ttl=56 time=11.338 ms
	64 bytes from 143.106.10.174: icmp_seq=5 ttl=56 time=13.077 ms
	64 bytes from 143.106.10.174: icmp_seq=6 ttl=56 time=10.975 ms
	64 bytes from 143.106.10.174: icmp_seq=7 ttl=56 time=27.322 ms
	64 bytes from 143.106.10.174: icmp_seq=8 ttl=56 time=12.342 ms
	64 bytes from 143.106.10.174: icmp_seq=9 ttl=56 time=14.947 ms
	
	--- cerejeira.unicamp.br ping statistics ---
	10 packets transmitted, 10 packets received, 0.0% packet loss
	round-trip min/avg/max/stddev = 10.975/17.779/50.932/11.970 ms
	~~~
	
	* Tempos de ida e volta mínimo: 10.975 ms
	* Médio: 17.779 ms
	* Máximo: 50.932 ms
	* Os tempos foram evidentemente menores, o que é de se esperar. Isso se deve à localização geográfica dos servidores que hospedam o website. Da UNICAMP, geograficamente perto de campinas, tem round-tripe time bem menor.

* [Laboratório de Redes de Computadores da UNICAMP](www.unicamp.br):

	~~~bash
	➜  Downloads  ping -c 10 www.lrc.ic.unicamp.br
	PING lrc-gw.ic.unicamp.br (143.106.7.163): 56 data bytes
	Request timeout for icmp_seq 0
	Request timeout for icmp_seq 1
	Request timeout for icmp_seq 2
	Request timeout for icmp_seq 3
	Request timeout for icmp_seq 4
	Request timeout for icmp_seq 5
	Request timeout for icmp_seq 6
	Request timeout for icmp_seq 7
	Request timeout for icmp_seq 8
	
	--- lrc-gw.ic.unicamp.br ping statistics ---
	10 packets transmitted, 0 packets received, 100.0% packet loss
	~~~
	* Vemos que não foi possível 'pingar' esse host, embora seja possível acessá-lo através de um navegador convencional. Por esse motivo, o ping não pode ser usado isoladamente como única medida para verificar se um dado website/host está online ou offline.


## ifconfig	
~~~bash
➜  Downloads  ifconfig
lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 0  (Loopback Local)
        RX packets 29947217  bytes 10788434066 (10.0 GiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 29947217  bytes 10788434066 (10.0 GiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

p4p1: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 143.106.16.163  netmask 255.255.255.192  broadcast 143.106.16.191
        inet6 fe80::feaa:14ff:fef7:b4d5  prefixlen 64  scopeid 0x20<link>
        ether fc:aa:14:f7:b4:d5  txqueuelen 1000  (Ethernet)
        RX packets 279827410  bytes 113542921863 (105.7 GiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 416942480  bytes 300140928132 (279.5 GiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
~~~
O endereço IP das interfaces de rede pertinentes são:

* p4p1 : 143.106.16.163, RX(bytes recebidos):
113542921863 TX(bytes enviados): 300140928132
* lo : 127.0.0.1 - interface de loopback, RX(bytes recebidos):
10788434066 TX(bytes enviados): 10788434066

Após executar o comando ping na interface de loopback, temos:

~~~bash
-bash-4.3$ ping -c 2 127.0.0.1
PING 127.0.0.1 (127.0.0.1) 56(84) bytes of data.
64 bytes from 127.0.0.1: icmp_seq=1 ttl=64 time=0.026 ms
64 bytes from 127.0.0.1: icmp_seq=2 ttl=64 time=0.026 ms

--- 127.0.0.1 ping statistics ---
2 packets transmitted, 2 received, 0% packet loss, time 999ms
rtt min/avg/max/mdev = 0.026/0.026/0.026/0.000 ms
-bash-4.3$ ifconfig lo
lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 0  (Loopback Local)
        RX packets 29947221  bytes 10788434402 (10.0 GiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 29947221  bytes 10788434402 (10.0 GiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0       
~~~

A interface lo, teve um incremento de 4 pacotes tanto em RX como TX, embora tenha sido enviado apenas 2. Isso é fácil de perceber, pois a interface funciona como loopback do sistema, logo tanto os pacotes enviados como recebidos são enviados por ela, daí o número 4(cada pacote conta a ida e a volta).

##route
~~~bash
-bash-4.3$ route
Tabela de Roteamento IP do Kernel
Destino         Roteador        MáscaraGen.    Opções Métrica Ref   Uso Iface
default         routeric3.lab.i 0.0.0.0         UG    0      0        0 p4p1
143.106.16.128  0.0.0.0         255.255.255.192 U     0      0        0 p4p1
link-local      0.0.0.0         255.255.0.0     U     1002   0        0 p4p1
~~~
Estão definidas 3 rotas na estação. A interface padrão de saída é a 'p4p1'.

##nslookup
~~~bash
-bash-4.3$ nslookup -type=any www.google.com
Server:		143.106.16.144
Address:	143.106.16.144#53

Non-authoritative answer:
www.google.com	has AAAA address 2800:3f0:4001:802::2004
Name:	www.google.com
Address: 216.58.202.4

Authoritative answers can be found from:
google.com	nameserver = ns4.google.com.
google.com	nameserver = ns1.google.com.
google.com	nameserver = ns2.google.com.
google.com	nameserver = ns3.google.com.
ns1.google.com	internet address = 216.239.32.10
ns2.google.com	internet address = 216.239.34.10
ns3.google.com	internet address = 216.239.36.10
ns4.google.com	internet address = 216.239.38.10
~~~
O endereço ip do host www.google.com é 216.58.202.4, mas existem vários servidores que são autoridade para esse ip. Já o servidor utilizado como DNS solver para a máquina é o servidor com endereço ip 143.106.16.144.

~~~bash
-bash-4.3$ nslookup 127.0.0.1
Server:		143.106.16.144
Address:	143.106.16.144#53

1.0.0.127.in-addr.arpa	name = localhost.
~~~
O nome associado é "localhost". Esse endereço é um endereço especial da interface de loopback, isso é, se refere à própria máquina.

##traceroute
* Google:

~~~bash
-bash-4.3$ traceroute www.google.com
traceroute to www.google.com (216.58.202.4), 30 hops max, 60 byte packets
 1  * * *
 2  143.106.16.150 (143.106.16.150)  0.119 ms  0.117 ms  0.127 ms
 3  143.106.7.129 (143.106.7.129)  0.550 ms  0.556 ms  0.551 ms
 4  area3-gw.unicamp.br (143.106.1.129)  0.544 ms  0.847 ms  0.852 ms
 5  ptp-nct-nbs.unicamp.br (143.106.199.13)  0.516 ms ptp-ncc-nbs.unicamp.br (143.106.199.9)  0.532 ms ptp-nct-nbs.unicamp.br (143.106.199.13)  0.504 ms
 6  as15169.sp.ix.br (187.16.216.55)  3.349 ms  3.262 ms  3.292 ms
 7  209.85.248.74 (209.85.248.74)  3.730 ms  3.463 ms  3.472 ms
 8  216.239.58.221 (216.239.58.221)  3.591 ms  3.537 ms  3.580 ms
 9  gru06s26-in-f4.1e100.net (216.58.202.4)  3.405 ms  3.489 ms  3.340 ms 
~~~

Existem 9 roteadores entre o host e o destino, sendo que o primeiro foi configurado para não responder a requisições ICMP(vemos pois ele passou o pacote para frente, mas não respondeu ao tracerout). É possível estimar que no mínimo 3 roteadores são brasileiros, pela terminação br(possivelmente os anteriores também são). O último(gru06s26-in-f4.1e100.net) é do google.

* cam.ca.uk:

~~~bash
-bash-4.3$ traceroute www.cam.ac.uk
traceroute to www.cam.ac.uk (131.111.150.25), 30 hops max, 60 byte packets
 1  * * *
 2  143.106.16.150 (143.106.16.150)  0.077 ms  0.077 ms  0.084 ms
 3  143.106.7.129 (143.106.7.129)  0.503 ms  0.509 ms  0.504 ms
 4  area3-gw.unicamp.br (143.106.1.129)  0.537 ms  0.767 ms  0.773 ms
 5  ptp-nct-nbs.unicamp.br (143.106.199.13)  9.460 ms ptp-ncc-nbs.unicamp.br (143.106.199.9)  10.024 ms ptp-nct-nbs.unicamp.br (143.106.199.13)  9.457 ms
 6  * * *
 7  sp-sp2.bkb.rnp.br (200.143.253.37)  3.823 ms  3.425 ms  2.916 ms
 8  br-rnp.redclara.net (200.0.204.213)  3.598 ms  3.629 ms  3.454 ms
 9  redclara.lon.uk.geant.net (62.40.124.36)  199.888 ms  199.032 ms  198.842 ms
10  janet-gw.mx1.lon.uk.geant.net (62.40.124.198)  198.840 ms  199.014 ms  199.081 ms
11  ae29.londpg-sbr1.ja.net (146.97.33.2)  199.378 ms  199.783 ms  199.781 ms
12  ae30.londtw-sbr1.ja.net (146.97.33.6)  200.259 ms  199.852 ms  202.364 ms
13  146.97.38.18 (146.97.38.18)  202.823 ms  202.793 ms  202.702 ms
14  146.97.65.117 (146.97.65.117)  203.044 ms  202.842 ms  202.836 ms
15  University-of-Cambridge.cambab-rbr1.eastern.ja.net (146.97.130.2)  202.741 ms  202.702 ms  210.766 ms
16  b-ec.c-mi.net.cam.ac.uk (192.84.5.93)  202.935 ms  202.918 ms  202.917 ms
17  c-mi.d-we.net.cam.ac.uk (192.84.5.98)  204.335 ms  204.178 ms  204.033 ms
18  primary.admin.cam.ac.uk (131.111.150.25)  203.474 ms  203.437 ms  203.088 ms
~~~

Nesse caso, existem 18 roteadores entre os destinos, no quais 5 são comuns com a primeira rota.
É fácil observar o instante que os pacotes chegam em um enlace transatlântico observando a diferença brusca entre os RTT(round-trip time - latência). No caso, entre os enlaces 8 e 9, existe um pulo de 3 ms para 199 ms, aproximadamente, o que indica grandes distâncias.

* home.pl:

~~~bash
-bash-4.3$ traceroute home.pl
traceroute to home.pl (212.85.96.1), 30 hops max, 60 byte packets
 1  * * *
 2  143.106.16.150 (143.106.16.150)  0.074 ms  0.073 ms  0.484 ms
 3  143.106.7.129 (143.106.7.129)  0.499 ms  0.495 ms  0.490 ms
 4  area3-gw.unicamp.br (143.106.1.129)  6.195 ms  6.339 ms  6.402 ms
 5  ptp-ncc-nbs.unicamp.br (143.106.199.9)  4.189 ms  4.204 ms  4.213 ms
 6  * * *
 7  mia1-sp-kyat.bkbrnp.br (200.143.255.89)  108.574 ms  108.582 ms  108.577 ms
 8  38.88.165.73 (38.88.165.73)  108.984 ms  108.760 ms  108.908 ms
 9  4.68.110.169 (4.68.110.169)  108.676 ms  108.617 ms  108.513 ms
10  ae-1-9.bar1.Warsaw1.Level3.net (4.69.153.70)  251.008 ms  251.010 ms  250.971 ms
11  LWLcom-Bremen.level3.net (213.242.117.58)  243.084 ms  242.969 ms  242.550 ms
12  * * *
13  * * *
14  * * *
15  * * *
16  * * *
17  * * *
18  * * *
19  * * *
20  * * *
21  * * *
22  * * *
23  * * *
24  * * *
25  * * *
26  * * *
27  * * *
28  * * *
29  * * *
30  * * *
~~~

Neste caso existem 30 roteadores entre o cliente e o destino. Rodando o trace route pelo website home.pl, vemos que a rota utilizada é bem diferente até chegar na UNICAMP, a partir daí a rota é a mesma:

~~~
HOST: vmy3.home.net.pl            Loss%   Snt   Last   Avg  Best  Wrst StDev
  1.|-- adx01.home.net.pl          0.0%     5    0.6   0.7   0.6   0.9   0.1
  2.|-- 62.129.251.154             0.0%     5    0.6   0.7   0.5   1.2   0.3
  3.|-- te0-0-1-5.rcr21.b016833-0  0.0%     5    1.3   1.4   1.2   1.6   0.1
  4.|-- be2882.ccr21.waw01.atlas.  0.0%     5    2.1   2.0   1.8   2.1   0.1
  5.|-- be2252.ccr41.ham01.atlas.  0.0%     5   14.4  14.4  14.2  14.4   0.1
  6.|-- be2815.ccr41.ams03.atlas.  0.0%     5   23.1  23.0  22.9  23.1   0.1
  7.|-- be12194.ccr41.lon13.atlas  0.0%     5   98.3  98.5  98.3  98.8   0.2
  8.|-- be2317.ccr41.jfk02.atlas.  0.0%     5  104.8 104.7 104.2 105.0   0.3
  9.|-- be2806.ccr41.dca01.atlas.  0.0%     5  108.5 109.2 108.5 109.7   0.5
 10.|-- be2112.ccr41.atl01.atlas. 20.0%     5  119.8 120.1 119.4 120.5   0.5
 11.|-- be2122.ccr21.mia01.atlas.  0.0%     5  133.7 134.2 133.7 135.0   0.6
 12.|-- be2054.ccr21.mia03.atlas.  0.0%     5  133.5 132.9 131.8 133.7   0.8
 13.|-- 38.88.165.74               0.0%     5  135.5 136.4 135.4 139.5   1.7
 14.|-- sp-mia1-kyat.bkbrnp.br     0.0%     5  242.1 241.4 240.9 242.1   0.5
 15.|-- rnp-nct.unicamp.br        20.0%     5  262.2 247.0 241.6 262.2  10.1
 16.|-- ptp-nbs-nct.unicamp.br    20.0%     5  243.8 243.6 243.4 243.8   0.2
 17.|-- ic-gw.unicamp.br          20.0%     5  239.7 239.9 239.5 240.6   0.5
 18.|-- ic3-gw.ic.unicamp.br      20.0%     5  242.2 242.7 242.2 243.2   0.5
 19.|-- xaveco.lab.ic.unicamp.br  20.0%     5  243.4 243.2 243.0 243.4   0.2
~~~

##netstat
Ao abrir o site da unicamp e rodar o netstat, foi possível perceber as seguintes conexões sendo abertas:

~~~
tcp4       0      0  192.168.1.101.64574    cerejeira.unicam.http  ESTABLISHED
tcp4       0      0  192.168.1.101.64573    cerejeira.unicam.http  ESTABLISHED
tcp4       0      0  192.168.1.101.64572    cerejeira.unicam.http  ESTABLISHED
tcp4       0      0  192.168.1.101.64571    cerejeira.unicam.http  ESTABLISHED
tcp4       0      0  192.168.1.101.64570    cerejeira.unicam.http  ESTABLISHED
~~~
##telnet
Podemos conectar no servidor HTTP do google através do cliente telnet com o seguinte comando:

~~~
➜  ~  telnet www.google.com 80
Trying 216.58.222.36...
Connected to www.google.com.
Escape character is '^]'.
~~~

Podemos executar um GET, por exemplo, dando a seguinte saída:

~~~
HTTP/1.0 302 Found
Cache-Control: private
Content-Type: text/html; charset=UTF-8
Location: http://www.google.com.br/?gfe_rd=cr&ei=bmrjVpmGPIyq8weUua2QAw
Content-Length: 262
Date: Sat, 12 Mar 2016 01:01:34 GMT

<HTML><HEAD><meta http-equiv="content-type" content="text/html;charset=utf-8">
<TITLE>302 Moved</TITLE></HEAD><BODY>
<H1>302 Moved</H1>
The document has moved
<A HREF="http://www.google.com.br/?gfe_rd=cr&amp;ei=bmrjVpmGPIyq8weUua2QAw">here</A>.
</BODY></HTML>
Connection closed by foreign host.
~~~

Não é possível conectar ao ip 127.0.0.1/localhost, pois não existe um servidor capaz respondendo às chamadas/pacotes nesse endereço.