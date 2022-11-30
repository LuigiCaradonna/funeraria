<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class MaterialSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $materials = [
            ['name' => 'Rosé del Portogallo'],
            ['name' => 'Travertino Stuccato'],
            ['name' => 'Travertino Resinato'],
            ['name' => 'Carrara'],
            ['name' => 'Botticino']
        ];

        DB::table('materials')->insert($materials);
    }
}
