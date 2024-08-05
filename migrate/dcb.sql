CREATE TABLE public.dcb (
	id int4 DEFAULT main.incid('public'::character varying, 'dcb'::character varying) NOT NULL,
	nome varchar(255) NOT NULL,
	CONSTRAINT dcb_pkey PRIMARY KEY (id)
);