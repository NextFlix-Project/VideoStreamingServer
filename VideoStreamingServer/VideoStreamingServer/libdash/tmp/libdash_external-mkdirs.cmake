# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/criffle/Desktop/CapstoneProject/VideoStreamingServer/VideoStreamingServer/VideoStreamingServer/libdash/src/libdash"
  "/home/criffle/Desktop/CapstoneProject/VideoStreamingServer/VideoStreamingServer/VideoStreamingServer/libdash/src/libdash_external-build"
  "/home/criffle/Desktop/CapstoneProject/VideoStreamingServer/VideoStreamingServer/VideoStreamingServer/libdash"
  "/home/criffle/Desktop/CapstoneProject/VideoStreamingServer/VideoStreamingServer/VideoStreamingServer/libdash/tmp"
  "/home/criffle/Desktop/CapstoneProject/VideoStreamingServer/VideoStreamingServer/VideoStreamingServer/libdash/src/libdash_external-stamp"
  "/home/criffle/Desktop/CapstoneProject/VideoStreamingServer/VideoStreamingServer/VideoStreamingServer/libdash/src"
  "/home/criffle/Desktop/CapstoneProject/VideoStreamingServer/VideoStreamingServer/VideoStreamingServer/libdash/src/libdash_external-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/criffle/Desktop/CapstoneProject/VideoStreamingServer/VideoStreamingServer/VideoStreamingServer/libdash/src/libdash_external-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/criffle/Desktop/CapstoneProject/VideoStreamingServer/VideoStreamingServer/VideoStreamingServer/libdash/src/libdash_external-stamp${cfgdir}") # cfgdir has leading slash
endif()
