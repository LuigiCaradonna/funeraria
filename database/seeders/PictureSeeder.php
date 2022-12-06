<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\DB;

class PictureSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $pictures = [
            ['name' => ' - '],
            ['name' => 'Ceramica applicata'],
            ['name' => 'Ceramica scavo'],
            ['name' => 'Cristallo applicato'],
            ['name' => 'Cristallo scavo'],
            ['name' => 'Cristallo TN applicato'],
            ['name' => 'Cristallo TN scavo']
        ];

        DB::table('pictures')->insert($pictures);
    }
}
