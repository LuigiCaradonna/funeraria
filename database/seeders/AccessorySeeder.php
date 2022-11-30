<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class AccessorySeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $accessories = [
            ['name' => 'Calice', 'accessory_type_id' => 1],
            ['name' => 'Vaschetta', 'accessory_type_id' => 1],
            ['name' => 'Mezza tazza', 'accessory_type_id' => 1],
            ['name' => 'Lumino', 'accessory_type_id' => 2],
            ['name' => 'Anello', 'accessory_type_id' => 2],
            ['name' => 'Cubetto', 'accessory_type_id' => 2],
            ['name' => 'Classica', 'accessory_type_id' => 3],
            ['name' => 'Rosa semifrontale', 'accessory_type_id' => 3],
            ['name' => 'Rosa semifrontale iride', 'accessory_type_id' => 3],
        ];

        DB::table('accessories')->insert($accessories);
    }
}
