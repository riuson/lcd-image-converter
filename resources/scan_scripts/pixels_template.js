var __internal_points_count = data.count;

for (var __internal_point_index = 0; __internal_point_index < __internal_points_count; __internal_point_index++) {
    var argb_uint32 = data.pixel(__internal_point_index);
    {
        %1
    }
    data.setPixel(__internal_point_index, argb_uint32);
}
