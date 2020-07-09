#!/usr/bin/env python
# encoding: utf-8
import sys
import os
import fnmatch
import glob
sys.path.insert(0, sys.path[0]+'/waf_tools')

VERSION = '1.0.0'
APPNAME = 'robot_box2d'

srcdir = '.'
blddir = 'build'

from waflib.Build import BuildContext
from waflib import Logs
from waflib.Tools import waf_unit_test
import boost
import eigen
import corrade
import magnum
import magnum_integration
import magnum_plugins



def options(opt):
    opt.load('compiler_cxx')
    opt.load('compiler_c')
    opt.load('boost')
    opt.load('eigen')
    opt.load('corrade')
    opt.load('magnum')
    opt.load('magnum_integration')
    opt.load('magnum_plugins')

    opt.add_option('--shared', action='store_true', help='build shared library', dest='build_shared')
    
    
def configure(conf):
    conf.get_env()['BUILD_GRAPHIC'] = False

    conf.load('compiler_cxx')
    conf.load('compiler_c')
    conf.load('waf_unit_test')
    conf.load('boost')
    conf.load('eigen')
    conf.load('corrade')
    conf.load('magnum')
    conf.load('magnum_integration')
    conf.load('magnum_plugins')

    conf.check_boost(lib='regex system filesystem unit_test_framework', min_version='1.46')
    conf.check_eigen(required=True)

    conf.check_corrade(components='Utility PluginManager', required=False)
    conf.env['magnum_dep_libs'] = 'MeshTools Primitives Shaders SceneGraph GlfwApplication'
    if conf.env['DEST_OS'] == 'darwin':
        conf.env['magnum_dep_libs'] += ' WindowlessCglApplication'
    else:
        conf.env['magnum_dep_libs'] += ' WindowlessGlxApplication'
    conf.check_magnum(components=conf.env['magnum_dep_libs'], required=False)


    conf.get_env()['BUILD_MAGNUM'] = True
    conf.env['magnum_libs'] = magnum.get_magnum_dependency_libs(conf, conf.env['magnum_dep_libs']) 

    

    native = ' -mavx -msse -msse2 '
    native_icc = ' '



    conf.env['lib_type'] = 'cxxstlib'
    if conf.options.build_shared:
        conf.env['lib_type'] = 'cxxshlib'
    

    if conf.env.CXX_NAME in ["icc", "icpc"]:
        common_flags = "-Wall -std=c++11"
        opt_flags = " -O3 -xHost -unroll -g -fpermissive" + native_icc
    elif conf.env.CXX_NAME in ["clang"]:
        common_flags = "-Wall -std=c++11"
        opt_flags = " -O3 -g -faligned-new -fpermissive" + native
    else:
        gcc_version = int(conf.env['CC_VERSION'][0]+conf.env['CC_VERSION'][1])
        if gcc_version < 47:
            common_flags = "-Wall -std=c++0x"
        else:
            common_flags = "-Wall -std=c++11"
        opt_flags = " -O3 -g -fpermissive" + native
        if gcc_version >= 71:
            opt_flags = opt_flags + " -faligned-new"

    all_flags = common_flags + opt_flags
    conf.env['CXXFLAGS'] = conf.env['CXXFLAGS'] + all_flags.split(' ')



def build(bld):

    
    # Quick and dirty link to box2d
    bld.env.LIBPATH_BOX2D = '/workspace/lib/'
    bld.env.LIB_BOX2D = [ 'box2d']
    bld.env.INCLUDES_BOX2D = '/workspace/include/'

    
    path = bld.path.abspath() + '/res'


    files = []
    magnum_files = []
    for root, dirnames, filenames in os.walk(bld.path.abspath()+'/src/robox2d/'):
        for filename in fnmatch.filter(filenames, '*.cpp'):
            ffile = os.path.join(root, filename)
            if 'robox2d/gui/magnum' in ffile:
                magnum_files.append(ffile)
            else:
                files.append(ffile)

    files = [f[len(bld.path.abspath())+1:] for f in files]
    robox2d_srcs = " ".join(files)
    magnum_files = [f[len(bld.path.abspath())+1:] for f in magnum_files]
    robox2d_magnum_srcs = " ".join(magnum_files)

    libs = 'BOOST EIGEN BOX2D'

    bld.program(features = 'cxx ' + bld.env['lib_type'],
                source = robox2d_srcs,
                includes = './src',
                uselib = libs,
                target = 'Robox2d')


    build_graphic = False

    if bld.get_env()['BUILD_MAGNUM'] == True:
        bld.program(features = 'cxx ' + bld.env['lib_type'],
                    source = robox2d_magnum_srcs,
                    includes = './src',
                    uselib = bld.env['magnum_libs'] + libs,
                    use = 'Robox2d',
                    target = 'Robox2dMagnum')

        build_graphic = True


    
    
    if build_graphic == True:
        bld.program(features = 'cxx',
                    install_path = None,
                    source = 'src/examples/arm.cpp',
                    includes = './src',
                    uselib = bld.env['magnum_libs'] + libs,
                    use = 'Robox2d Robox2dMagnum',
                    defines = ['GRAPHIC'],
                    target = 'arm_graphic')

    bld.program(features = 'cxx',
                install_path = None,
                source = 'src/examples/arm.cpp',
                includes = './src',
                uselib = bld.env['magnum_libs'] + libs,
                use = 'Robox2d Robox2dMagnum',
                target = 'arm_plain')



    install_files = []
    for root, dirnames, filenames in os.walk(bld.path.abspath()+'/src/robox2d/'):
        for filename in fnmatch.filter(filenames, '*.hpp'):
            install_files.append(os.path.join(root, filename))
    install_files = [f[len(bld.path.abspath())+1:] for f in install_files]

    for f in install_files:
        end_index = f.rfind('/')
        if end_index == -1:
            end_index = len(f)
        bld.install_files('${PREFIX}/include/' + f[4:end_index], f)
    if bld.env['lib_type'] == 'cxxstlib':
        bld.install_files('${PREFIX}/lib', blddir + '/libRobox2d.so')
        if bld.get_env()['BUILD_MAGNUM'] == True:
            bld.install_files('${PREFIX}/lib', blddir + '/libRobox2dMagnum.so')
    else:
        # OSX/Mac uses .dylib and GNU/Linux .so
        suffix = 'dylib' if bld.env['DEST_OS'] == 'darwin' else 'so'
        bld.install_files('${PREFIX}/lib', blddir + '/libRobox2d.' + suffix)
        if bld.get_env()['BUILD_MAGNUM'] == True:
            bld.install_files('${PREFIX}/lib', blddir + '/libRobox2dMagnum.' + suffix)
