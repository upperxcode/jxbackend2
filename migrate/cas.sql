CREATE TABLE public.cas (
	id int4 DEFAULT main.incid('public'::character varying, 'cas'::character varying) NOT NULL,
	nome varchar(255) NOT NULL,
	CONSTRAINT cas_pkey PRIMARY KEY (id)
);