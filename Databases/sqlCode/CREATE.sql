create database Restaurant;
use Restaurant;

create table if not exists PIATA(
	onoma varchar(30) not null,
	timi smallint null,
	xronos tinyint null,
	odigies text null,
	primary key (onoma)
)ENGINE = InnoDB;

create table if not exists KIRIWS_PIATO(
	onoma_piatou varchar(30) not null,
	eidos_piatou enum('zimarika','pizza','rizoto','nioki'),
	megethos enum('4','8','16') null,
	eidos_pastas varchar(10) null,
	constraint kiriws
	foreign key(onoma_piatou) references PIATA(onoma)
	on delete cascade on update cascade
)ENGINE = InnoDB;

create table if not exists SALATA(
	onoma_piatou varchar(30) not null,
	primary key (onoma_piatou),
	constraint salata
	foreign key(onoma_piatou) references PIATA(onoma)
	on delete cascade on update cascade
)ENGINE = InnoDB;

create table if not exists OREKTIKA(
	onoma_piatou varchar(30) not null,
	primary key (onoma_piatou),
	constraint orektika
	foreign key(onoma_piatou) references PIATA(onoma)
	on delete cascade on update cascade
)ENGINE = InnoDB;

create table if not exists SOUPA(
	onoma_piatou varchar(30) not null,
	primary key (onoma_piatou),
	constraint soupa
	foreign key(onoma_piatou) references PIATA(onoma)
	on delete cascade on update cascade
)ENGINE = InnoDB;


create table if not exists PIATA_IMERAS(
	diathesimes_merides smallint not null,
	onoma_soupas varchar(30) not null,
	onoma_orektikou varchar(30) not null,
	onoma_kuriws varchar(30) not null,
	imerominia date not null,

	constraint soup
	foreign key(onoma_soupas) references SOUPA(onoma_piatou)
	on delete cascade on update cascade,

	constraint start
	foreign key(onoma_orektikou) references OREKTIKA(onoma_piatou)
	on delete cascade on update cascade,

	constraint main
	foreign key(onoma_kuriws) references KIRIWS_PIATO(onoma_piatou)
	on delete cascade on update cascade,
	
	primary key(imerominia)


)ENGINE = InnoDB;


create table if not exists TRAPEZIA(
	id smallint not null auto_increment,
	perigrafi enum('parathiro','aithrio','kipos','sala'),
	xwritikotita enum('4','8','12'),
	primary key(id)
)ENGINE = InnoDB;

create table if not exists PARAGELIA(
	id smallint not null auto_increment,
	trapezi smallint not null,
	imerominia date,
	atoma smallint,
	wra time,
	
	primary key(id),

	constraint tables
	foreign key(trapezi) references TRAPEZIA(id)
	on delete cascade on update cascade
	
)ENGINE = InnoDB;


create table if not exists POTA(
	onoma varchar(20) not null,
	timi smallint not null, 
	eidos enum('nero','anapsiktiko','ximos','krasi','mpira'),
	
	primary key(onoma)

)ENGINE = InnoDB;

create table if not exists PERILAMVANEI_POTA(
	id_paragelias smallint not null auto_increment,
	onoma_potou varchar(20) not null,
	posotita smallint not null,
	
	constraint includes
	foreign key(id_paragelias) references PARAGELIA(id)
	on delete cascade on update cascade,

	constraint includes2
	foreign key(onoma_potou) references POTA(onoma)
	on delete cascade on update cascade
	

)ENGINE = InnoDB;

create table if not exists SISTATIKA(
	onoma varchar(30) not null,
	orio_diathesimotitas int not null,
	diathesimi_posotita int not null,
	auxisi_timis float  null, 

	primary key(onoma)

)ENGINE = InnoDB;

create table if not exists PELATIS(
	id int not null auto_increment,
	onoma varchar(30) not null,
	pontoi int, 
	paragelies_mina int,
	
	primary key(id)

)ENGINE = InnoDB;

create table if not exists YPALLILOS(
	id smallint not null auto_increment,
	misthos int not null,
	imerominia_proslipsis date,
	onoma varchar(30) not null,
	meres_ergasias tinyint, 
	vardies tinyint, 
	ptixeio varchar(40),
	CV text, 
	eti_proipiresias tinyint, 
	wriaio_kostos smallint,

	primary key(id)

)ENGINE = InnoDB;

create table if not exists SERVITOROI(
	id smallint not null,
	id_trapeziou smallint not null,
	
	primary key (id,id_trapeziou),
	
	constraint serve1
	foreign key(id) references YPALLILOS(id)
	on delete cascade on update cascade,

	constraint serve2
	foreign key(id_trapeziou) references TRAPEZIA(id)
	on delete cascade on update cascade
	
)ENGINE = InnoDB;


create table if not exists SERVITOROI_MONO(
	id smallint not null,
	
	primary key (id),
	
	constraint se
	foreign key(id) references YPALLILOS(id)
	on delete cascade on update cascade

	
)ENGINE = InnoDB;


create table if not exists MAGEIRES(
	id smallint not null,

	primary key (id),
	
	constraint mag
	foreign key(id) references YPALLILOS(id)
	on delete cascade on update cascade

)ENGINE = InnoDB;





create table if not exists KATHARISTES(
	id smallint not null,
	
	primary key (id),
	
	constraint kath
	foreign key(id) references YPALLILOS(id)
	on delete cascade on update cascade

)ENGINE = InnoDB;

create table if not exists YPAL_TAMEIOY(
	id smallint not null,
	
	primary key (id),
	
	constraint tam
	foreign key(id) references YPALLILOS(id)
	on delete cascade on update cascade

)ENGINE = InnoDB;

create table if not exists THLEPHONITES(
	id smallint not null,
	
	primary key (id),
	
	constraint til
	foreign key(id) references YPALLILOS(id)
	on delete cascade on update cascade

)ENGINE = InnoDB;

create table if not exists YPALL_PARAGELIWN(
	id smallint not null,
	
	primary key (id),
	
	constraint par
	foreign key(id) references YPALLILOS(id)
	on delete cascade on update cascade

)ENGINE = InnoDB;


create table if not exists YPALL_VARDIAS(
	id smallint not null,
	
	primary key (id),
	
	constraint vard
	foreign key(id) references YPALLILOS(id)
	on delete cascade on update cascade

)ENGINE = InnoDB;

create table if not exists SISTATIKA_PIATWN(
	onoma_piatou varchar(30) not null,
	sistatiko varchar(30) not null,
	
	primary key (onoma_piatou,sistatiko),

	constraint piato
	foreign key(onoma_piatou) references PIATA(onoma)
	on delete cascade on update cascade,

	constraint sistatiko
	foreign key(sistatiko) references SISTATIKA(onoma)
	on delete cascade on update cascade

)ENGINE = InnoDB;




create table if not exists VARDIA(
	imerominia date not null, 
	tipos enum('prwini','vradini'),
	servitorosA smallint not null,
	servitorosB smallint not null,
	MageirasA smallint not null,
	MageirasB smallint not null,
	MageirasC smallint not null,
	KatharistisA smallint not null,
	KatharistisB smallint not null,
	Ypal_tameiou smallint not null,
	Tilephonitis smallint not null,
	Ypal_parageliwnA smallint not null,
	Ypal_parageliwnB smallint not null,
	Ypeuth_Vardias smallint not null,

	primary key(imerominia,tipos),

	constraint s1
	foreign key(servitorosA) references SERVITOROI(id)
	on delete cascade on update cascade,

	constraint s2
	foreign key(servitorosB) references SERVITOROI(id)
	on delete cascade on update cascade,

	constraint m1
	foreign key(MageirasA) references MAGEIRES(id)
	on delete cascade on update cascade,

	constraint m2
	foreign key(MageirasB) references MAGEIRES(id)
	on delete cascade on update cascade,

	constraint m3
	foreign key(MageirasC) references MAGEIRES(id)
	on delete cascade on update cascade,

	constraint k1
	foreign key(KatharistisA) references KATHARISTES(id)
	on delete cascade on update cascade,

	constraint k2
	foreign key(KatharistisB) references KATHARISTES(id)
	on delete cascade on update cascade,

	constraint t1
	foreign key(Ypal_tameiou) references YPAL_TAMEIOY(id)
	on delete cascade on update cascade,
	
	constraint p1
	foreign key(Ypal_parageliwnA) references YPALL_PARAGELIWN(id)
	on delete cascade on update cascade,

	constraint p2
	foreign key(Ypal_parageliwnB) references YPALL_PARAGELIWN(id)
	on delete cascade on update cascade,

	constraint v1
	foreign key(Ypeuth_Vardias) references YPALL_VARDIAS(id)
	on delete cascade on update cascade

)ENGINE = InnoDB;


create table if not exists WRES_ERGASIAS(
	imerominia date not null,
	id smallint not null,
	wres smallint not null,

	primary key(imerominia,id),

	constraint 
	foreign key(id) references YPALLILOS(id)
	on delete cascade on update cascade

)ENGINE = InnoDB;

create table if not exists PERILAMVANEI_PIATA(
	id_epilogis smallint not null auto_increment,
	id_paragelias smallint not null,
	onoma_piatou varchar(20) not null,	
	posotita smallint not null,
	kommatia enum("4","8","16") null,
	
	primary key(id_epilogis),

	constraint plates_contains
	foreign key(id_paragelias) references PARAGELIA(id)
	on delete cascade on update cascade,

	constraint plates_contains2
	foreign key(onoma_piatou) references PIATA(onoma)
	on delete cascade on update cascade


)ENGINE = InnoDB;

create table if not exists EPIPLEON_SISTATIKA(
	id_epilogis smallint not null,
	sistatiko varchar(30) not null,
	
	primary key(id_epilogis,sistatiko),
	
	constraint epipleon
	foreign key(id_epilogis) references PERILAMVANEI_PIATA(id_epilogis)
	on delete cascade on update cascade,

	constraint epipleon2
	foreign key(sistatiko) references SISTATIKA(onoma)
	on delete cascade on update cascade

)ENGINE = InnoDB;

create table if not exists ENIMEROSH_SERVITORWN(
	
	id_paragelias smallint not null,

	primary key(id_paragelias)
	

)ENGINE = InnoDB;
