from distutils.core import setup, Extension

naive = Extension('dumbpy',
                    include_dirs=['.'],
                    sources = ['python/dumbpy.c', 'naive/matrix.c'],
                    extra_compile_args = ["-g", "-Wall", "-std=gnu99"],
                  )

performance = Extension('numc',
                          include_dirs=['.'],
                          sources = ['python/numc.c', 'performance/matrix.c'],
                          extra_compile_args = ["-g", "-Wall", "-std=gnu99", "-fopenmp", "-mavx"],
                          extra_link_args=['-lgomp'],
                        )

setup (name = 'dumbpy',
       version = '1.0',
       description = 'dumbpy contains all of the methods needed for matrix math, but they are slow',
       ext_modules = [naive],)

setup (name = 'numc',
       version = '1.0',
       description = 'numc contains all of the methods needed for matrix math, but they are fast',
       ext_modules = [performance],)

#setup (name = '61c',
#       version = '1.0',
#       description = 'numc contains all of the methods needed for matrix math, but they are fast',
#       ext_modules = [performance, naive],)
