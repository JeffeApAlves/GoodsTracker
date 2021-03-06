#!/usr/bin/env python

"""

@file    vanet.py
@author  Jefferson Alves
@date    2018-01-09
@version 0.1

"""

from __future__ import absolute_import
from __future__ import print_function

import os
import os.path
import locale
import sys
import subprocess
import click
import shlex
import xml.etree.ElementTree as ET
from distutils import *
from project import *

SUMO_HOME = os.environ.get("SUMO_HOME", os.path.join(
    os.path.dirname(os.path.abspath(__file__)), ".."))

typemapdir = os.path.join(SUMO_HOME, "data", "typemap")

sys.path.append(os.path.join(SUMO_HOME, 'tools'))

import osmGet
import randomTrips


typemaps = {
    "net": os.path.join(typemapdir, "osmNetconvert.typ.xml"),
    "poly": os.path.join(typemapdir, "osmPolyconvert.typ.xml"),
    "urban": os.path.join(typemapdir, "osmNetconvertUrbanDe.typ.xml"),
    "urbanBrSP": os.path.join(typemapdir, "osmNetconvertUrbanBrSP.typ.xml"),
    "pedestrians": os.path.join(typemapdir, "osmNetconvertPedestrians.typ.xml"),
    "ships": os.path.join(typemapdir, "osmNetconvertShips.typ.xml"),
    "bicycles": os.path.join(typemapdir, "osmNetconvertBicycle.typ.xml"),
}


locale.setlocale(locale.LC_ALL, '')


class SUMO(object):

    # local onde esta instalado o SUMO
    HOME = SUMO_HOME
    # diretorio das ferramentas do SUMO
    TOOLDIR = HOME + '/tools'
    # nome da simulação
    NAME = PROJECT.configuration["vanet"]["name"]
    # diretorio de trabalho
    OUTPUTDIR = "%s/%s" % (PROJECT.VANETDIR , NAME)
    # Prefixo dos arquivos do projeto
    PREFIX_FILE=NAME
    # Arquivo de configuração  usado na criação dos poligonos adicionais
    POLY_CONFIG=PREFIX_FILE+'.polycfg'
    # Arquivo de configuração  usado na criação das vias
    NET_CONFIG= PREFIX_FILE + '.netccfg'
    # Arquivo de configuração  usado na simulação
    SUMO_CONFIG=PREFIX_FILE + '.sumocfg'
    # Arquivo do mapa (entrada)
    OSM_FILE = PREFIX_FILE + '_bbox.osm.xml'
    # arquivo do mapa de entrada
    INPUT_OSM_FILE='./' + OSM_FILE
    # Coordenadas que define a região do mapa para simulação
    BBOX = "%.4f,%.4f,%.4f,%.4f" %( PROJECT.configuration["vanet"]["bbox"]["bottom"],
                                    PROJECT.configuration["vanet"]["bbox"]["left"],
                                    PROJECT.configuration["vanet"]["bbox"]["top"],
                                    PROJECT.configuration["vanet"]["bbox"]["right"])
                            
    ## Informações para geração da frota/tráfico da simulação

    # Classes/tipos que serão criadas
    TYPES = PROJECT.configuration["vanet"]["types"]
    # Fim do periodo de criação de trips
    ENDTIME = PROJECT.configuration["vanet"]["endtime"]
    # Periodo do ciclo de criação das trips (ex: a cada x criar n trips)
    PERIOD = PROJECT.configuration["vanet"]["period"]
    # Influencia na estatistica onde será iniciado a trips
    FRINGE_FACTOR  = PROJECT.configuration["vanet"]["fringe_factor"]
    # Seed para referencia na geração randomica da simulação
    SEED = PROJECT.configuration["vanet"]["seed"]

    ## Arquivos de saida
    NET_FILE = PREFIX_FILE+'.net.xml'
    POLY_FILE = PREFIX_FILE+'.poly.xml'
    GUI_FILE = PREFIX_FILE+'.view.xml'
    LAUNCH_FILE = PREFIX_FILE+'.launch.xml'
    ADD_FILE = PREFIX_FILE+'.type.add.xml'

    @staticmethod
    def create_simulation():

        SUMO.update_all_paths()
        
        if not os.path.exists(SUMO.OUTPUTDIR):
            os.mkdir(SUMO.OUTPUTDIR)

        SUMO.create_guicfg()
        SUMO.create_polycfg()
        SUMO.create_netcfg()  
        SUMO.download_map()
        SUMO.create_net()      
        SUMO.create_poly()     
        SUMO.create_all_trips()
        SUMO.create_sumocfg()
        SUMO.create_launch()
        

    @staticmethod
    def update_simulation():

        update_all_paths()

        # Classes/tipos que serão criadas
        # TYPES = PROJECT.configuration["vanet"]["types"]

        create_all_trips()
        

    @staticmethod
    def update_all_paths():

        # Atualiza nome de todos os arquivos conforme o nome do projeto
        PREFIX_FILE=SUMO.NAME
 
        # Arquivo de configuração  usado na criação dos poligonos adicionais
        POLY_CONFIG=SUMO.PREFIX_FILE+'.polycfg'
        # Arquivo de configuração  usado na criação das vias
        NET_CONFIG= SUMO.PREFIX_FILE + '.netccfg'
        # Arquivo de configuração  usado na simulação
        SUMO_CONFIG=SUMO.PREFIX_FILE + '.sumocfg'
        # Arquivo do mapa (entrada)
        OSM_FILE = SUMO.PREFIX_FILE + '_bbox.osm.xml'
    
        # Arquivos de saida
        NET_FILE = PREFIX_FILE+'.net.xml'
        POLY_FILE = PREFIX_FILE+'.poly.xml'
        GUI_FILE = PREFIX_FILE+'.view.xml'
        LAUNCH_FILE = PREFIX_FILE+'.launch.xml'
        ADD_FILE = PREFIX_FILE+'.type.add.xml'
 
        # diretorio de trabalho
        SUMO.OUTPUTDIR = PROJECT.VANETDIR + '/' + SUMO.NAME
        

    @staticmethod
    def create_guicfg():

        filename = SUMO.OUTPUTDIR + '/' + SUMO.GUI_FILE

        viewsettings = ET.Element("viewsettings")

        ET.SubElement(viewsettings, "scheme", name="real world")
        ET.SubElement(viewsettings, "delay", value="50")
        ET.SubElement(viewsettings, "viewport",zoom="2000",x="2500",y="6200")

        tree = ET.ElementTree(viewsettings)
        tree.write(filename,xml_declaration='<?xml version="1.0"?>',encoding='utf8')
        

    @staticmethod
    def create_polycfg():

        filename = SUMO.OUTPUTDIR + '/' + SUMO.POLY_CONFIG

        # TODO O atributo original deveria ser o da linha comentado (xsi)
        # config = ET.Element("configuration",xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance",xsi:noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd")
        config = ET.Element("configuration",xmlns="http://www.w3.org/2001/XMLSchema-instance",noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd")

        input = ET.SubElement(config, "input")
        ET.SubElement(input, "net-file" , value = SUMO.NET_FILE)
        ET.SubElement(input, "osm-files" , value = SUMO.OSM_FILE)
        ET.SubElement(input, "osm.keep-full-type" , value = "true")
        ET.SubElement(input, "type-file" , value = "%s/data/typemap/osmPolyconvert.typ.xml" % SUMO.HOME)

        output = ET.SubElement(config, "output")
        ET.SubElement(output, "output-file" , value = SUMO.POLY_FILE)

        report = ET.SubElement(config, "report")
        ET.SubElement(report, "verbose" , value = "true")

        tree = ET.ElementTree(config)
        tree.write(filename,xml_declaration='<?xml version="1.0"?>',encoding='utf8')
        

    @staticmethod
    def create_netcfg():

        filename = SUMO.OUTPUTDIR + '/' + SUMO.NET_CONFIG

        dir_typemap = '%s/data/typemap' % SUMO.HOME

        # TODO O atributo original deveria ser o da linha comentado (xsi)
        # config = ET.Element("configuration",xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance",xsi:noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd")
        config = ET.Element("configuration",xmlns="http://www.w3.org/2001/XMLSchema-instance",noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd")

        input = ET.SubElement(config, "input")
        ET.SubElement(input, "osm-files" , value = SUMO.OSM_FILE)
        ET.SubElement(input, "type-files" , value = "%s,%s,%s" % (typemaps['net'],typemaps['urbanBrSP'],typemaps['pedestrians']))

        output = ET.SubElement(config, "output")
        ET.SubElement(output, "output-file" , value = SUMO.NET_FILE)
        ET.SubElement(output, "output.street-names" , value = "true")
        ET.SubElement(output, "output.original-names" , value = "true")

        tls_building = ET.SubElement(config, "processing")
        ET.SubElement(tls_building, "tls.discard-loaded" ,  value = "false")
        ET.SubElement(tls_building, "tls.discard-simple" ,  value = "false")
        ET.SubElement(tls_building, "tls.join" ,            value = "true")
        ET.SubElement(tls_building, "tls.join-dist" ,       value = "40")
        ET.SubElement(tls_building, "tls.guess-signals" ,   value = "false")

        ramp_guessing = ET.SubElement(config, "ramp_guessing")
        ET.SubElement(ramp_guessing, "ramps.guess" ,  value = "true")

        processing = ET.SubElement(config, "processing")
        ET.SubElement(processing, "geometry.remove", value="true")
        ET.SubElement(processing, "roundabouts.guess", value="false")
        ET.SubElement(processing, "junctions.join", value="true")
        ET.SubElement(processing, "junctions.join-dist", value="5")
        ET.SubElement(processing, "junctions.corner-detail", value="5")
        ET.SubElement(processing, "crossings.guess", value="true")
        ET.SubElement(processing, "sidewalks.guess", value="false")

        building = ET.SubElement(config, "building")
        ET.SubElement(building, "default.junctions.radius" , value = "1")

        report = ET.SubElement(config, "report")
        ET.SubElement(report, "verbose" , value = "true")

        tree = ET.ElementTree(config)
        tree.write(filename,xml_declaration='<?xml version="1.0"?>',encoding='utf8')


    @staticmethod
    def create_poly():

        cl = 'polyconvert --configuration-file %s/%s' % (SUMO.OUTPUTDIR,SUMO.POLY_CONFIG)
        args = shlex.split(cl)
        subprocess.call(args)
        

    @staticmethod
    def download_map():

        # ['-b', '-23.650530497027194,-46.760725116729915,-23.636393920054857,-46.72111425399774', '-p', 'osm']

        # ['--bbox','-46.7600,-23.6500,-46.7000,-23.6350', '--prefix', 'sp', '--output-dir', '/media/jefferson/Dados/workspace/gtracker/vanet/sp']

        args = ["--bbox",SUMO.BBOX,"--prefix",SUMO.PREFIX_FILE,"--output-dir",SUMO.OUTPUTDIR]

        click.echo(args)

        osmGet.get(args)


    @staticmethod
    def create_net():

        cl = 'netconvert  --configuration-file %s/%s' % (SUMO.OUTPUTDIR,SUMO.NET_CONFIG)
        args = shlex.split(cl)
        click.echo(args)
        subprocess.call(args)


    @staticmethod
    def create_all_trips():

        for type_class in SUMO.TYPES :
            SUMO.create_trips(type_class)
        

    @staticmethod
    def create_trips(type_class):

        pre_fix = type_class[:3]
        route_file = '%s/%s.%s.rou.xml' % (SUMO.OUTPUTDIR,SUMO.PREFIX_FILE,type_class)
        trip_file = '%s/%s.%s.trips.xml' % (SUMO.OUTPUTDIR,SUMO.PREFIX_FILE,type_class)
        net_file = '%s/%s' % (SUMO.OUTPUTDIR,SUMO.NET_FILE)
        fringe_factor = int(SUMO.FRINGE_FACTOR[type_class])
        end_time = int(SUMO.ENDTIME[type_class])
        period = int(SUMO.PERIOD[type_class])

        if os.path.isfile(net_file) :

            if type_class ==  "pedestrian" :

                cl = '--seed %d  --pedestrians --max-distance 3000 --fringe-factor %d  --prefix %s -n %s -r %s -p %d -e %d -o %s' % (SUMO.SEED,fringe_factor,pre_fix,net_file,route_file,period,end_time,trip_file)

            else :

                cl = '--seed %d  --vehicle-class %s --min-distance 1000 --fringe-factor %d  --prefix %s -n %s -r %s -p %d -e %d -o %s'  % (SUMO.SEED,type_class,fringe_factor,pre_fix,net_file,route_file,period,end_time,trip_file)
            
            args = shlex.split(cl)

            randomTrips.main(randomTrips.get_options(args))


            '''
            if type_class ==  "pedestrian" :

                cl = '%s/randomTrips.py --seed %d  --pedestrians --max-distance 3000 --fringe-factor %d  --prefix %s -n %s -r %s -p %d -e %d -o %s' % (SUMO.TOOLDIR,SUMO.SEED,fringe_factor,pre_fix,net_file,route_file,period,end_time,trip_file)

            else :

                cl = '%s/randomTrips.py --seed %d  --vehicle-class %s --min-distance 1000 --fringe-factor %d  --prefix %s -n %s -r %s -p %d -e %d -o %s'  % (SUMO.TOOLDIR,SUMO.SEED,type_class,fringe_factor,pre_fix,net_file,route_file,period,end_time,trip_file)
            
            args = shlex.split(cl)

            process = subprocess.Popen(args,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
            
            stdout, erro = process.communicate()

            if stdout :
                click.echo(stdout)
                click.echo("Rotas[%s]: %s %s) - done" % (type_class,route_file,trip_file))

            if erro :
                click.echo(erro)
                click.echo("ERROR: Falha na criação das rotas para %s / arquivo  %s " % (type_class,route_file))

            '''
        else:
            click.echo("ERROR: Não encontrado o arquivo %s para criação das rotas" % net_file)
        
       
    @staticmethod
    def create_launch():

    	# Cria o arquivo xml que omnet utilizara para iniciar o sumo

        filename = SUMO.OUTPUTDIR + '/' + SUMO.LAUNCH_FILE

        launch = ET.Element("launch")

        ET.SubElement(launch, "copy", file=SUMO.NET_FILE)

        for t in SUMO.TYPES:
            ET.SubElement(launch, "copy", file="%s.%s.rou.xml" % (SUMO.PREFIX_FILE,t) )

        ET.SubElement(launch, "copy", file=SUMO.POLY_FILE)
        ET.SubElement(launch, "copy", file=SUMO.GUI_FILE)
        ET.SubElement(launch, "copy", file=SUMO.SUMO_CONFIG , type= "config")

        tree = ET.ElementTree(launch)
        tree.write(filename,xml_declaration='<?xml version="1.0"?>',encoding='utf8')


    @staticmethod
    def create_sumocfg():

        # Cria arquivo xml com a configuração para ser usada na simulação

        route_files = ','.join(str(SUMO.PREFIX_FILE + '.' + e + '.rou.xml') for e in SUMO.TYPES)

        filename = SUMO.OUTPUTDIR + '/' + SUMO.SUMO_CONFIG

        # TODO O atributo original deveria ser o da linha comentado (xsi)
        # config = ET.Element("configuration",xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance",xsi:noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd")
        config = ET.Element("configuration",xmlns="http://www.w3.org/2001/XMLSchema-instance",noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/sumoConfiguration.xsd")


        input = ET.SubElement(config, "input")
        ET.SubElement(input, "net-file" , value = SUMO.NET_FILE)
        ET.SubElement(input, "route-files" , value = route_files)
        ET.SubElement(input, "additional-files" , value = SUMO.POLY_FILE)

        processing = ET.SubElement(config, "processing")
        ET.SubElement(processing, "ignore-route-errors" , value = "true")

        routing = ET.SubElement(config, "processing")
        ET.SubElement(routing, "device.rerouting.adaptation-steps" , value = "100")

        report = ET.SubElement(config, "report")
        ET.SubElement(report, "verbose" , value = "true")
        ET.SubElement(report, "duration-log.statistics" , value = "true")
        ET.SubElement(report, "no-step-log" , value = "true")

        gui = ET.SubElement(config, "gui")
        ET.SubElement(gui, "gui-settings-file" , value = SUMO.GUI_FILE)

        tree = ET.ElementTree(config)
        tree.write(filename,xml_declaration='<?xml version="1.0"?>',encoding='utf8')

    @staticmethod
    def run():
        cl = 'sumo-gui -c %s/%s' % (SUMO.OUTPUTDIR,SUMO.SUMO_CONFIG)
        args = shlex.split(cl)
        subprocess.call(args)

