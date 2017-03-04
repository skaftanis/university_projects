#1 (Για τις μέρες 01-01-2015 - 02-01-2015)
select YPALLILOS.onoma,sum(wres) as "wres",YPALLILOS.wriaio_kostos*sum(wres) as "kostise" from WRES_ERGASIAS,YPALLILOS where (WRES_ERGASIAS.imerominia="2015-01-01" or WRES_ERGASIAS.imerominia="2015-01-02") and YPALLILOS.id=WRES_ERGASIAS.id group by WRES_ERGASIAS.id;

#2 (για αυτά που η διαθέσιμη ποσότητα είναι μικρότερη (<) από το όριο διαθεσιμότητας
select onoma from SISTATIKA where diathesimi_posotita<orio_diathesimotitas;

#3
select SISTATIKA_PIATWN.onoma_piatou from SISTATIKA_PIATWN,SISTATIKA where SISTATIKA_PIATWN.sistatiko=SISTATIKA.onoma and SISTATIKA.diathesimi_posotita=0;

#4
select TRAPEZIA.id,TRAPEZIA.perigrafi,TRAPEZIA.xwritikotita,YPALLILOS.onoma from TRAPEZIA,SERVITOROI,YPALLILOS,VARDIA where (TRAPEZIA.xwritikotita="8" or TRAPEZIA.xwritikotita="12") and TRAPEZIA.id=id_trapeziou and SERVITOROI.id=YPALLILOS.id and VARDIA.tipos="vradini" and (SERVITOROI.id=VARDIA.servitorosA or SERVITOROI.id=VARDIA.servitorosB);


#5 
create view temp as select onoma_piatou,sum(posotita) as athroisma,imerominia from PERILAMVANEI_PIATA inner join PARAGELIA on PERILAMVANEI_PIATA.id_paragelias=PARAGELIA.id group by onoma_piatou;
select max(athroisma),imerominia,onoma_piatou from temp group by imerominia;


#6
create view kostos_potwn as select PERILAMVANEI_POTA.id_paragelias, sum(PERILAMVANEI_POTA.posotita*POTA.timi) as sinolo_potwn from PERILAMVANEI_POTA, POTA where PERILAMVANEI_POTA.onoma_potou=POTA.onoma group by id_paragelias;

create view kostos_piatwn as select PERILAMVANEI_PIATA.id_paragelias, sum(PERILAMVANEI_PIATA.posotita*PIATA.timi) as sinolo_piatwn from PERILAMVANEI_PIATA, PIATA where PERILAMVANEI_PIATA.onoma_piatou=PIATA.onoma group by id_paragelias;

create view sinolo_ana_atomo as select kostos_potwn.id_paragelias,(sum(kostos_piatwn.sinolo_piatwn)+sum(kostos_potwn.sinolo_potwn))/PARAGELIA.atoma as sinolo from kostos_piatwn,kostos_potwn,PARAGELIA where kostos_piatwn.id_paragelias=kostos_potwn.id_paragelias and PARAGELIA.id=kostos_potwn.id_paragelias and PARAGELIA.id=kostos_piatwn.id_paragelias group by kostos_potwn.id_paragelias;


select id_paragelias as paragelia_megaliterou_kostous_ana_atomo from sinolo_ana_atomo where sinolo=(select max(sinolo) from sinolo_ana_atomo); 
select id_paragelias as paragelia_mikroterou_kostous_ana_atomo from sinolo_ana_atomo where sinolo=(select min(sinolo) from sinolo_ana_atomo); 


#7 (από 12 έως 14-02-2015)
create view KOSTOS_PARAGELIAS as select kostos_potwn.id_paragelias,sum(kostos_piatwn.sinolo_piatwn)+sum(kostos_potwn.sinolo_potwn) as sinolo from kostos_piatwn,kostos_potwn where kostos_piatwn.id_paragelias=kostos_potwn.id_paragelias group by kostos_potwn.id_paragelias;

select sum(sinolo)/count(id_paragelias) as meso_kostos from KOSTOS_PARAGELIAS, PARAGELIA where PARAGELIA.IMEROMINIA>="2015-02-12" and PARAGELIA.IMEROMINIA<="2015-02-14" and PARAGELIA.id=KOSTOS_PARAGELIAS.id_paragelias;

#8 (2015-02-14)
create view PIATA_ANA_PARAGELIA as select id_paragelias, sum(posotita) as piata from PERILAMVANEI_PIATA group by id_paragelias;
select MAGEIRES_PARAGELIA.id, PIATA_ANA_PARAGELIA.piata from MAGEIRES_PARAGELIA, PIATA_ANA_PARAGELIA where MAGEIRES_PARAGELIA.id_oloklipomenis_paragelias=PIATA_ANA_PARAGELIA.id_paragelias and MAGEIRES_PARAGELIA.imerominia="2015-02-14";

#9 (ο σερβιτόρος με το id=5 τη πρωινή βάρδια στις 2015-02-20) 
select count(*) as paragelies from SERVITOROI_PARAGELIA where imerominia="2015-02-20" and id=5;

#10
select onoma from PELATIS where paragelies_mina>10;

#10+1 Εμφάνισε τα πιάτα που περιλαμβάνονται σε κάθε παραγγελία ταξινομημέμενα με την ώρα ώστε ο μάγειρας να βλέπει πρώτα τη πιο παλιά. 
select * from PERILAMVANEI_PIATA inner join PARAGELIA on PARAGELIA.id=PERILAMVANEI_PIATA.id_paragelias order by imerominia,wra;

#10+2 Πληροφορίες που χρειάζεται να βλέπει ο μάγειρας
select SISTATIKA_PIATWN.onoma_piatou,posotita,kommatia,xronos,odigies from SISTATIKA_PIATWN left join PERILAMVANEI_PIATA on SISTATIKA_PIATWN.onoma_piatou=PERILAMVANEI_PIATA.onoma_piatou inner join PIATA on PERILAMVANEI_PIATA.onoma_piatou=PIATA.onoma;
















