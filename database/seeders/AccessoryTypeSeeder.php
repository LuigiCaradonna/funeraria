<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class AccessoryTypeSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $accessory_types = [
            ['name' => 'Vaso'],
            ['name' => 'Portalampada'],
            ['name' => 'Fiamma'],
            ['name' => 'Cornice']
        ];

        DB::table('accessory_types')->insert($accessory_types);
    }
}
