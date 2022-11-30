<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class SculptureSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $sculptures = [
            ['name' => 'HB1042'],
            ['name' => 'HB1031'],
            ['name' => 'HB1023'],
            ['name' => 'HB2001'],
            ['name' => 'HB2041'],
            ['name' => 'Padre Pio Rose']
        ];

        DB::table('sculptures')->insert($sculptures);
    }
}
