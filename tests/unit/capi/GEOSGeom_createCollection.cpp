// 
// Test Suite for C-API GEOSGeom_createCollection

#include <tut.hpp>
// geos
#include <geos_c.h>
// std
#include <array>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

namespace tut
{
    //
    // Test Group
    //

    // Common data used in test cases.
    struct test_capigeosgeom_createcollection_data
    {
        GEOSContextHandle_t handle_;
        GEOSGeom geom_; // collection result
        enum { geom_size = 3 };

        static void notice(const char *fmt, ...)
        {
            std::fprintf( stdout, "NOTICE: ");

            va_list ap;
            va_start(ap, fmt);
            std::vfprintf(stdout, fmt, ap);
            va_end(ap);
        
            std::fprintf(stdout, "\n");
        }

        test_capigeosgeom_createcollection_data()
            : geom_(0), handle_(initGEOS_r(notice, notice))
        {
        }

        ~test_capigeosgeom_createcollection_data()
        {
            GEOSGeom_destroy(geom_); geom_ = 0;
            finishGEOS_r(handle_);
        }
    };

    typedef test_group<test_capigeosgeom_createcollection_data> group;
    typedef group::object object;

    group test_capigeosgeom_createcollection_group("capi::GEOSGeom_createCollection");

    //
    // Test Cases
    //

    // Create collection from constant length C-array
    template<>
    template<>
    void object::test<1>()
    {
        GEOSGeom geoms[geom_size];
        geoms[0] = GEOSGeom_createEmptyPoint_r(handle_);
        geoms[1] = GEOSGeom_createEmptyPoint_r(handle_);
        geoms[2] = GEOSGeom_createEmptyPoint_r(handle_);
        // takes ownership of individual geometries
        geom_ = GEOSGeom_createCollection_r(handle_, GEOS_MULTIPOINT, geoms, geom_size);
        ensure_equals(GEOSGetNumGeometries_r(handle_, geom_), geom_size);
    }

    // Create collection from constant length std::array
    template<>
    template<>
    void object::test<2>()
    {
        std::array<GEOSGeom, geom_size> geoms = {
        GEOSGeom_createEmptyPoint_r(handle_),
        GEOSGeom_createEmptyPoint_r(handle_),
        GEOSGeom_createEmptyPoint_r(handle_)
        };
        // takes ownership of individual geometries
        geom_ = GEOSGeom_createCollection_r(handle_, GEOS_MULTIPOINT,
            geoms.data(), static_cast<int>(geoms.size()));
        ensure_equals(GEOSGetNumGeometries_r(handle_, geom_), geom_size);
    }

    // Create collection from dynamic length std::vector of geometries
    template<>
    template<>
    void object::test<3>()
    {
        std::vector<GEOSGeom> geoms;
        geoms.push_back(GEOSGeom_createEmptyPoint_r(handle_));
        geoms.push_back(GEOSGeom_createEmptyPoint_r(handle_));
        geoms.push_back(GEOSGeom_createEmptyPoint_r(handle_));
        geoms.push_back(GEOSGeom_createEmptyPoint_r(handle_));
        geoms.push_back(GEOSGeom_createEmptyPoint_r(handle_));
        // takes ownership of individual geometries
        geom_ = GEOSGeom_createCollection_r(handle_, GEOS_MULTIPOINT,
            geoms.data(), static_cast<int>(geoms.size()));
        ensure_equals(GEOSGetNumGeometries_r(handle_, geom_), geoms.size());
    }

} // namespace tut

